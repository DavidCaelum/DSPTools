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

#ifndef DSPTOOLS_GAIN_HEADER_INCLUDED
#define DSPTOOLS_GAIN_HEADER_INCLUDED

#include "AudioEffect.h"

namespace DSPTools {

template <typename type>
class Gain : AudioEffect<type>
{
public:
    Gain() {}
    ~Gain() {}
    
    /** Setup the gain. This must be called before calling processAudio.
    */
    void setup(double sampleRate, int maxBufferSize, int numChannels)
    {
        smoothedGain.setup(sampleRate, numChannels, 0.0, 0.05);
        setDecibelRange(-100.0, 0.0);
    }
    
    /** Process a buffer of audio with the gain.
    */
    void processAudio(AudioBufferInfo<type>& audioBuffer)
    {
        for (int channel = 0; channel < audioBuffer.getNumChannels(); ++channel) {
            auto data = audioBuffer.getChannelData(channel);
            for (int sample = 0; sample < audioBuffer.getNumSamples(); ++sample) {
                data[sample] *= smoothedGain.getNextModulatedParameterValue(channel, sample);
            }
        }
    }
    
    /** Set the gain value in dBFS.
    */
    void setDecibels(type dB, type modAmount = 0.0)
    {
        smoothedGain.setParameterValue((dB >= -100.0) ? Maths<type>::decibelsToAmplitude(dB) : 0.0, (modAmount < 0.0) ? sqrt(abs(modAmount)) * -1.0 : sqrt(modAmount));
    }
    
    /** Set the range of the gain parameter in dBFS.
    */
    void setDecibelRange(type minDB, type maxDB)
    {
        smoothedGain.setParameterRange((minDB >= -100.0) ? Maths<type>::decibelsToAmplitude(minDB) : 0.0, (maxDB >= -100.0) ? Maths<type>::decibelsToAmplitude(maxDB) : 0.0);
    }
    
    /** Set the modulation source for the gain parameter.
    */
    void setGainModulationSource(std::shared_ptr<ModulationSource<type>> modulationSource)
    {
        smoothedGain.setModulationSource(modulationSource);
    }
    
private:
    ModulationParameter<type> smoothedGain;
};

} // namespace DSPTools

#endif // DSPTOOLS_GAIN_HEADER_INCLUDED
