/*MIT License

Copyright (c) 2022 David Antonia

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#ifndef DSPTOOLS_ENVELOPE_FOLLOWER_HEADER_INCLUDED
#define DSPTOOLS_ENVELOPE_FOLLOWER_HEADER_INCLUDED

#include "Maths.h"

namespace DSPTools {

template <typename type>
class EnvelopeFollower
{
    public:
    enum Mode {
        peak = 0,
        rms = 1
    };
    
    EnvelopeFollower() {}
    ~EnvelopeFollower() {}
    
    /** Setup the envelope follower.
    */
    void setup(double sampleRate, int numChannels, Mode mode)
    {
        assert(sampleRate > 0.0);
        assert(numChannels > 0);
        this->mode = mode;
        this->sampleRate = sampleRate;
        calculateAttackCoefficient(attack);
        calculateReleaseCoefficient(release);
        lastOut.resize(numChannels);
        for (int channel = 0; channel < numChannels; ++channel) {
            lastOut[channel] = 0.0;
        }
    }
    
    /** Get the attack value of the envelope in seconds.
    */
    type getAttack()
    {
        return attack;
    }
    
    /** Get the release value of the envelope in seconds.
    */
    type getRelease()
    {
        return release;
    }
    
    /** Set the attack value of the envelope in seconds.
    */
    void setAttack(type value)
    {
        attack = value;
        calculateAttackCoefficient(value);
    }
    
    /** Get the release value of the envelope in seconds.
    */
    void setRelease(type value)
    {
        release = value;
        calculateReleaseCoefficient(value);
    }
    
    /** Set the envelope detection mode.
    */
    void setMode(Mode newMode)
    {
        mode = newMode;
    }
    
    /** Calculate the envelope of a sample.
    */
    type calculateEnvelope(type value, int channel)
    {
        lastOut[channel] = (value > lastOut[channel]) ? calculateEnvelopeInAttack(value, channel) : calculateEnvelopeInRelease(value, channel);
        return lastOut[channel];
    }
    
private:
    
    type calculateEnvelopeInAttack(type value, int channel)
    {
        return (mode == rms) ?  sqrt(attackCoefficient * pow(lastOut[channel], 2.0) + (1.0 - attackCoefficient) * pow(value, 2.0))
                                : value + attackCoefficient * (lastOut[channel] - value);
    }
    
    type calculateEnvelopeInRelease(type value, int channel)
    {
        return (mode == rms) ? sqrt(releaseCoefficient * pow(lastOut[channel], 2.0) + (1.0 - releaseCoefficient) * pow(value, 2.0))
                                : value + releaseCoefficient * (lastOut[channel] - value);
    }
    
    void calculateAttackCoefficient(type time)
    {
        attackCoefficient = pow(Maths<type>::euler, -1.0 / (time * sampleRate));
    }
    
    void calculateReleaseCoefficient(type time)
    {
        releaseCoefficient = pow(Maths<type>::euler, -1.0 / (time * sampleRate));
    }
    
    type attackCoefficient = 0.0, releaseCoefficient = 0.0, attack = 0.01, release = 0.05;
    std::vector<type> lastOut;
    double sampleRate = 1.0;
    Mode mode = peak;
};

} // namespace DSPTools

#endif // DSPTOOLS_ENVELOPE_FOLLOWER_HEADER_INCLUDED
