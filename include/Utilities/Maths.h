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

#ifndef DSPTOOLS_MATHS_HEADER_INCLUDED
#define DSPTOOLS_MATHS_HEADER_INCLUDED

namespace DSPTools {

template <typename type>
class Maths
{
public:
    /** Constant for pi.
    */
    static constexpr type pi = static_cast<type> (3.141592653589793238L);
    /** Constant for Euler's number.
    */
    static constexpr type euler = static_cast<type> (2.71828182845904523536L);
    
    /** Converts dBFS to amplitude.
        Note: 1.0 = 0dBFS.
    */
    static type decibelsToAmplitude(type dB)
    {
        return std::pow (type (10.0), dB * type (0.05));
    }
    
    /** Converts amplitude to dBFS.
        Note: 1.0 = 0dBFS.
    */
    static type amplitudeToDecibels(type amplitude)
    {
        return static_cast<type> (std::log10 (amplitude)) * type (20.0);
    }
    
    /** Limit a value to a range.
        Note: assumes that the given limits are the correct way round.
    */
    static type limit(type lowerLimit, type upperLimit, type valueToLimit)
    {
        return valueToLimit < lowerLimit ? lowerLimit
                                             : (upperLimit < valueToLimit ? upperLimit
                                                                              : valueToLimit);
    }
    
    /** Calculate a sine wave based on a phase value from 0 to 1.
    */
    static type generateSine(type phase0to1)
    {
        return std::sin(phase0to1 * pi * 2.0);
    }
    
    /** Calculate a naive saw wave based on a phase value from 0 to 1.
    */
    static type generateSaw(type phase0to1)
    {
        return (-1.0 * (1.0 - phase0to1) + 1.0 * phase0to1) * -1.0;
    }

    /** Calculate a naive square wave based on a phase value from 0 to 1.
    */
    static type generateSquare(type phase0to1)
    {
        auto sine = generateSine(phase0to1);
        return (sine >= 0.0) ? 1.0 : -1.0;
    }
    
    /** Calculate a naive triangle wave based on a phase value from 0 to 1.
    */
    static type generateTriangle(type phase0to1)
    {
        if (phase0to1 < 0.25) {
            return phase0to1 * 4;
        }
        else if (phase0to1 < 0.75) {
            return 2.0 - (phase0to1 * 4.0);
        }
        else {
            return phase0to1 * 4 - 4.0;
        }
    }
};

} // namespace DSPTools

#endif // DSPTOOLS_MATHS_HEADER_INCLUDED
