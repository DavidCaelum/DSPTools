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

#pragma once

#ifndef DSPTOOLS_RANGE_HEADER_INCLUDED
#define DSPTOOLS_RANGE_HEADER_INCLUDED

#include "Maths.h"

namespace DSPTools {

template <typename type>
class Range
{
public:
    Range() {}
    ~Range() {}
    
    /** Set the minimum and maximum range.
    */
    void setRange(type min, type max)
    {
        minValue = min;
        maxValue = max;
    }
    
    /** Restrict a value to its range.
    */
    type constrainValueToRange(type value)
    {
        return Maths<type>::limit(minValue, maxValue, value);
    }
    
    /** Get the minimum value for the range.
    */
    type getMinValue()
    {
        return minValue;
    }
    
    /** Get the maximum value for the range.
    */
    type getMaxValue()
    {
        return maxValue;
    }
    
private:
    type minValue, maxValue;
};

} // namespace DSPTools

#endif // DSPTOOLS_RANGE_HEADER_INCLUDED
