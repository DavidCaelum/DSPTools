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

#ifndef DSPTOOLS_MODULATION_PARAMETER_HEADER_INCLUDED
#define DSPTOOLS_MODULATION_PARAMETER_HEADER_INCLUDED

#include "ModulationSource.h"
#include "../Utilities/Range.h"
#include "../Utilities/SmoothedValue.h"

namespace DSPTools {

template <typename type>
class ModulationParameter
{
public:
    ModulationParameter () {}
    ~ModulationParameter() {}
    
    /** Setup the parameter.
    */
    void setup(double sampleRate, int numChannels, type initialValue, type smoothingTime)
    {
        parameterValue.resize(numChannels);
        modulationValue.resize(numChannels);
        currentModulatedParameterValue.resize(numChannels);
        for (int channel = 0; channel < numChannels; ++channel) {
            parameterValue[channel].setup(sampleRate, initialValue, smoothingTime);
            modulationValue[channel].setup(sampleRate, 0.0, smoothingTime);
        }
    }
    
    /** Set the range for the parameter.
    */
    void setParameterRange(type minValue, type maxValue)
    {
        parameterRange.setRange(minValue, maxValue);
    }
    
    /** Set the value for the parameter as well as its modulation value.
    */
    void setParameterValue(type parameter, type modulation)
    {
        for (int channel = 0; channel < parameterValue.size(); ++channel) {
            parameterValue[channel].setTargetValue(parameter);
            modulationValue[channel].setTargetValue(modulation);
        }
    }
    
    /** Set the modulation source.
    */
    void setModulationSource(std::shared_ptr<ModulationSource<type>> modulationSource)
    {
            this->modulationSource = modulationSource;
    }
    
    /** Modulate a parameter value.
    */
    type getNextModulatedParameterValue(int channel, int sampleIndex)
    {
        type staticParameterValue = parameterValue[channel].getNextValue();
        type thisModulationValue = modulationValue[channel].getNextValue();
        
        if (!modulationSource)
        {
            currentModulatedParameterValue[channel] = staticParameterValue;
            return parameterRange.constrainValueToRange(staticParameterValue);
        }
        
        type modAmount = modulationSource->getModulationSample(sampleIndex) * thisModulationValue;
        
        if (modAmount != 0.0) {
            currentModulatedParameterValue[channel] = calculateModulatedParameter(staticParameterValue, Maths<type>::limit(-1.0, 1.0, modAmount));
        } else {
            currentModulatedParameterValue[channel] = staticParameterValue;
        }
        
        return parameterRange.constrainValueToRange(currentModulatedParameterValue[channel]);
    }
    
    /** Get the last output modulated parameter value.
    */
    type getCurrentModulatedParameterValue(int channel)
    {
        return parameterRange.constrainValueToRange(currentModulatedParameterValue[channel]);
    }
    
private:
    type calculateModulatedParameter(type currentValue, type modAmount)
    {
        return (modAmount > 0.0) ? currentValue + (parameterRange.getMaxValue() - currentValue) * modAmount : currentValue + (currentValue - parameterRange.getMinValue()) * modAmount;
    }
    
    std::shared_ptr<ModulationSource<type>> modulationSource;
    Range<type> parameterRange;
    std::vector<SmoothedValue<type>> parameterValue, modulationValue;
    std::vector<type> currentModulatedParameterValue;
};

} // namespace DSPTools

#endif // DSPTOOLS_MODULATION_PARAMETER_HEADER_INCLUDED
