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

#ifndef DSPTOOLS_PANNER_HEADER_INCLUDED
#define DSPTOOLS_PANNER_HEADER_INCLUDED

#include "AudioEffect.h"

namespace DSPTools {

template <typename type>
class Panner : AudioEffect<type>
{
public:
    Panner() {}
    ~Panner() {}
    
    /** Setup the panner. This must be called before calling processAudio.
    */
    void setup(double sampleRate, int maxBufferSize, int numChannels)
    {
        smoothedPanner.setup(sampleRate, numChannels, 0.0, 0.05);
        smoothedPanner.setParameterRange(-1.0, 1.0);
    }
    
    /** Process a buffer of audio with the panner.
    */
    void processAudio(AudioBufferInfo<type>& audioBuffer)
    {
        for (int channel = 0; channel < audioBuffer.getNumChannels(); ++channel) {
            auto data = audioBuffer.getChannelData(channel);
            for (int sample = 0; sample < audioBuffer.getNumSamples(); ++sample) {
                auto panPosition = smoothedPanner.getNextModulatedParameterValue(channel, sample);
                auto amp = calculateAmplitude(panPosition);
                data[sample] *= (channel == 0) ? sqrt(1.0 - amp) : sqrt(amp);
            }
        }
    }
    
    /** Set the pan value from -1 to 1.
    */
    void setPanning(type panPos0to1, type modAmount = 0.0)
    {
        smoothedPanner.setParameterValue(panPos0to1, modAmount);
    }
    
    /** Set the modulation source for the pan parameter.
    */
    void setPannerModulationSource(std::shared_ptr<ModulationSource<type>> modulationSource)
    {
        smoothedPanner.setModulationSource(modulationSource);
    }
    
private:
    type calculateAmplitude(type panPosition)
    {
        return panPosition / 2.0 + 0.5;
    }
    
    ModulationParameter<type> smoothedPanner;
};

} // namespace DSPTools

#endif // DSPTOOLS_PANNER_HEADER_INCLUDED
