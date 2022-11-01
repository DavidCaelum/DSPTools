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

#ifndef DSPTOOLS_COMPRESSOR_HEADER_INCLUDED
#define DSPTOOLS_COMPRESSOR_HEADER_INCLUDED

#include "../Utilities/EnvelopeFollower.h"

namespace DSPTools {

template <typename type>
class Compressor : AudioEffect<type>
{
public:
    Compressor() {}
    ~Compressor() {}
    
    /** Setup the compressor. This must be called before calling processAudio.
    */
    void setup(double sampleRate, int maxBufferSize, int numChannels)
    {
        follower.setup(sampleRate, numChannels, EnvelopeFollower<type>::Mode::rms);
        attack.setup(sampleRate, numChannels, 0.0, 0.05);
        release.setup(sampleRate, numChannels, 0.0, 0.05);
        threshold.setup(sampleRate, numChannels, 0.0, 0.05);
        ratio.setup(sampleRate, numChannels, 1.0, 0.05);
        knee.setup(sampleRate, numChannels, 0.0, 0.05);
        
        attack.setParameterRange(0.00001, 0.5);
        release.setParameterRange(0.00001, 0.5);
        threshold.setParameterRange(-100.0, 0.0);
        ratio.setParameterRange(1.0, 20.0);
        knee.setParameterRange(0.0, 1.0);
    }
    
    /** Process a buffer of audio with the compressor.
    */
    void processAudio(AudioBufferInfo<type>& audioBuffer)
    {
        for (int channel = 0; channel < audioBuffer.getNumChannels(); ++channel) {
            auto data = audioBuffer.getChannelData(channel);
            for (int sample = 0; sample < audioBuffer.getNumSamples(); ++sample) {
                follower.setAttack(attack.getNextModulatedParameterValue(channel, sample));
                follower.setRelease(release.getNextModulatedParameterValue(channel, sample));
                data[sample] *= Maths<type>::decibelsToAmplitude(calcGain(ratio.getNextModulatedParameterValue(channel, sample), threshold.getNextModulatedParameterValue(channel, sample), Maths<type>::amplitudeToDecibels(follower.calculateEnvelope(data[sample], channel)), knee.getNextModulatedParameterValue(channel, sample)));
            }
        }
    }
    
    /** Set the envelope detection mode.
    */
    void setEnvelopeType(typename EnvelopeFollower<type>::Mode mode)
    {
        follower.setMode(mode);
    }
    
    /** Set the attack value in seconds.
    */
    void setAttack(type time, type modAmount = 0.0)
    {
        attack.setParameterValue(time, modAmount);
    }

    /** Set the range of the attack parameter in seconds.
    */
    void setAttackRange(type minTime, type maxTime)
    {
        attack.setParameterRange(std::max(type(0.00001), minTime), maxTime);
    }
    
    /** Set the release value in seconds.
    */
    void setRelease(type time, type modAmount = 0.0)
    {
        release.setParameterValue(time, modAmount);
    }
    
    /** Set the range of the release parameter in seconds.
    */
    void setReleaseRange(type minTime, type maxTime)
    {
        release.setParameterRange(std::max(type(0.00001), minTime), maxTime);
    }
    
    /** Set the threshold value in dBFS.
    */
    void setThreshold(type thresholdInDb, type modAmount = 0.0)
    {
        threshold.setParameterValue(thresholdInDb, modAmount);
    }
    
    /** Set the range of the threshold parameter in dBFS.
    */
    void setThresholdRange(type minThresholdInDb, type maxThresholdInDb)
    {
        threshold.setParameterRange(minThresholdInDb, maxThresholdInDb);
    }
    
    /** Set the ratio value.
    */
    void setRatio(type ratioValue, type modAmount = 0.0)
    {
        ratio.setParameterValue(ratioValue, modAmount);
    }
    
    /** Set the range of the ratio parameter.
    */
    void setRatioRange(type minRatioValue, type maxRatioValue)
    {
        ratio.setParameterRange(minRatioValue, maxRatioValue);
    }
    
    /** Set the knee value.
    */
    void setKnee(type knee0To1, type modAmount = 0.0)
    {
        knee.setParameterValue(knee0To1, modAmount);
    }
    
    /** Set the modulation source for the attack parameter.
    */
    void setAttackModulationSource(std::shared_ptr<ModulationSource<type>> modulationSource)
    {
        attack.setModulationSource(modulationSource);
    }
    
    /** Set the modulation source for the release parameter.
    */
    void setReleaseModulationSource(std::shared_ptr<ModulationSource<type>> modulationSource)
    {
        release.setModulationSource(modulationSource);
    }
    
    /** Set the modulation source for the threshold parameter.
    */
    void setThresholdModulationSource(std::shared_ptr<ModulationSource<type>> modulationSource)
    {
        threshold.setModulationSource(modulationSource);
    }
    
    /** Set the modulation source for the ratio parameter.
    */
    void setRatioModulationSource(std::shared_ptr<ModulationSource<type>> modulationSource)
    {
        ratio.setModulationSource(modulationSource);
    }
    
    /** Set the modulation source for the knee parameter.
    */
    void setKneeModulationSource(std::shared_ptr<ModulationSource<type>> modulationSource)
    {
        knee.setModulationSource(modulationSource);
    }
    
private:
    type calcGain(type ratio, type thresholdInDb, type envelopeInDb, type knee)
    {
        auto kneeWidth = thresholdInDb * knee * -1.0;
        auto lowerKneeBound = thresholdInDb - (kneeWidth / 2.0);
        auto upperKneeBound = thresholdInDb + (kneeWidth / 2.0);
        auto slope = 1.0 - (1.0 / ratio);
        
        if (knee > 0.0 && envelopeInDb > lowerKneeBound && envelopeInDb < upperKneeBound) {
            slope *= (((envelopeInDb - lowerKneeBound) / kneeWidth) * 0.5);
            return slope * (lowerKneeBound - envelopeInDb);
        } else
        {
            slope *= (thresholdInDb - envelopeInDb);
            return std::min(0.0, slope);
        }
    }
    
    ModulationParameter<type> attack, release, threshold, ratio, knee;
    EnvelopeFollower<type> follower;
};


} // namespace DSPTools

#endif // DSPTOOLS_COMPRESSOR_HEADER_INCLUDED
