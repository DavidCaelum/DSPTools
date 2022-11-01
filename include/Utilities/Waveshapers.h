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

#ifndef DSPTOOLS_WAVESHAPERS_HEADER_INCLUDED
#define DSPTOOLS_WAVESHAPERS_HEADER_INCLUDED

#include "Maths.h"

namespace DSPTools {

template <typename type>
class Waveshapers
{
public:
    static type arraya(type input)
    {
        return (input > 1.0) ? 1.0 : (input < 1.0) ? -1.0 : ((3.0 * input) / 2.0) * (1.0 - (input * input) / 3.0);
    }
    
    static type sigmoid(type input)
    {
        return 2.0 * (1.0 / (1.0 + pow(Maths<type>::euler, -input))) - 1.0;
    }
    
    static type sigmoid2(type input)
    {
        type eToInput = pow(Maths<type>::euler, input);
        return ((eToInput - 1.0) * (Maths<type>::euler + 1.0)) / ((eToInput + 1.0) * (Maths<type>::euler - 1.0));
    }
    
    /** Estimation of a tanh() function.
    */
    static type tanHEstimate(type input)
    {
        input = Maths<type>::limit(-3.0506, 3.0506, input);
        return input / (1.0 + (input * input) / (3.0 + (input * input) / (5.0 + (input * input) / 7.0)));
    }
};

} // namespace DSPTools

#endif // DSPTOOLS_WAVESHAPERS_HEADER_INCLUDED
