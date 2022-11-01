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

#ifndef DSPTOOLS_BASIC_OSCILLATOR_HEADER_INCLUDED
#define DSPTOOLS_BASIC_OSCILLATOR_HEADER_INCLUDED

#include "Oscillator.h"

namespace DSPTools {

template <typename type>
class BasicOscillator : Oscillator<type>
{
public:
    enum Waveshape {
        Sine = 0,
        Triangle = 1,
        Square = 2,
        Saw = 3
    };
    
    BasicOscillator() {}
    ~BasicOscillator() {}
    
    /** Setup the oscillator.
    */
    void setup(double sampleRate)
    {
        this->sampleRate = sampleRate;
        phase = 0.0;
        increment = 0.1;
    }
    
    /** Set the oscillator frequency.
    */
    void setFrequency(type frequency)
    {
        increment = frequency / sampleRate;
    }
    
    /** Set the waveshape for the oscillator.
    */
    void setWaveshape(Waveshape waveshape)
    {
        currentWaveshape = waveshape;
    }
    
    /** Get the next sample value from the oscillator.
    */
    type getNextSample()
    {
        type value = 0.0;
        switch (currentWaveshape) {
            case Sine:
                value = Maths<type>::generateSine(phase);
                break;
            case Triangle:
                value = Maths<type>::generateTriangle(phase);
                break;
            case Square:
                value = Maths<type>::generateSquare(phase);
                break;
            case Saw:
                value = Maths<type>::generateSaw(phase);
                break;
            default:
                break;
        }
        
        phase += increment;
        while (phase >= 1.0) {
            phase -= 1.0;
        }
        
        return value;
    }
    
private:
    Waveshape currentWaveshape;
    type phase, increment;
    double sampleRate;
};

} // namespace DSPTools

#endif // DSPTOOLS_BASIC_OSCILLATOR_HEADER_INCLUDED
