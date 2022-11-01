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

#ifndef DSPTOOLS_WAVE_MODULATOR_HEADER_INCLUDED
#define DSPTOOLS_WAVE_MODULATOR_HEADER_INCLUDED

#include "ModulationSource.h"
#include "../AudioSources/BasicOscillator.h"

namespace DSPTools {

template <typename type>
class WaveModulator : public ModulationSource<type>
{
public:    
    WaveModulator() {}
    ~WaveModulator() {}
    
    /** Setup the modulation source.
    */
    void setup(int maxBufferSize, double sampleRate) override
    {
        ModulationSource<type>::setup(maxBufferSize, sampleRate);
        oscillator.setup(sampleRate);
    }
    
    /** Calculate the modulation samples for the length of the buffer.
    */
    void prepareModulationBuffer(int numSamples) override
    {
        for (int sample = 0; sample < numSamples; ++sample)
        {
            this->setModulationSample(sample, oscillator.getNextSample() * 0.5 + 0.5);
        }
    }
    
    /** Set the waveshape for the modulating oscillator.
    */
    void setModulationShape(typename BasicOscillator<type>::Waveshape waveshape)
    {
        oscillator.setWaveshape(waveshape);
    }
    
    /** Set the frequency for the modulating oscillator.
    */
    void setFrequency(type frequency)
    {
        oscillator.setFrequency(frequency);
    }
    
private:
    BasicOscillator<type> oscillator;
};

} // namespace DSPTools

#endif // DSPTOOLS_WAVE_MODULATOR_HEADER_INCLUDED
