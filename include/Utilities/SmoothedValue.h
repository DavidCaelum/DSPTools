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

#ifndef DSPTOOLS_SMOOTHED_VALUE_HEADER_INCLUDED
#define DSPTOOLS_SMOOTHED_VALUE_HEADER_INCLUDED

namespace DSPTools {

template <typename type>
class SmoothedValue {
public:
    SmoothedValue()
    {
        static_assert(std::is_floating_point<type>::value, "Smoothed Value: Not a floating point type.");
    }
    
    ~SmoothedValue() {}
    
    /** Setup the smoothed value.
    */
    void setup(double sampleRate, type initialValue, type smoothingTime)
    {
        assert(sampleRate > 0.0);
        this->sampleRate = sampleRate;
        this->smoothingTime = smoothingTime;
        currentValue = initialValue;
        targetValue = initialValue;
        incrementValue = 0.0;
        countdown = 0;
    }
    
    /** Set the value that should be smoothed towards.
    */
    void setTargetValue(type newTarget)
    {
        if (targetValue == newTarget) {
            return;
        }
        targetValue = newTarget;
        
        if (targetValue != currentValue)
        {
            if (smoothingTime > 0.0) {
                incrementValue = (targetValue - currentValue) / (smoothingTime * sampleRate);
                countdown = smoothingTime * sampleRate;
            } else
            {
                incrementValue = (targetValue - currentValue);
                countdown = 1;
            }
        }
    }
    
    /** Set the value that should be smoothed towards and skip to that value.
    */
    void setCurrentAndTargetValue(type value)
    {
        targetValue = value;
        currentValue = value;
        incrementValue = 0.0;
    }
    
    /** Set the next smoothed value.
    */
    type getNextValue()
    {
        if (countdown <= 0)
        {
            return currentValue;
        }
        
        --countdown;
        currentValue += incrementValue;
        
        return currentValue;
    }
    
    /** Get the value that is being smoothed towards.
    */
    type getTargetValue()
    {
        return targetValue;
    }
    
    /** Get the last output value.
    */
    type getCurrentValue()
    {
        return currentValue;
    }
    
private:
    type currentValue = 0.0, targetValue = 0.0, incrementValue = 0.0, smoothingTime = 0.0;
    unsigned int countdown = 0;
    double sampleRate = 1.0;
};

} // namespace DSPTools

#endif // DSPTOOLS_SMOOTHED_VALUE_HEADER_INCLUDED
