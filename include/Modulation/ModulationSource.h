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

#ifndef DSPTOOLS_MODULATION_SOURCE_HEADER_INCLUDED
#define DSPTOOLS_MODULATION_SOURCE_HEADER_INCLUDED

namespace DSPTools {

template <typename type>
class ModulationSource
{
public:
    /** Setup the modulation source.
    */
    virtual void setup(int maxBufferSize, double sampleRate)
    {
        samples.resize(maxBufferSize);
        this->sampleRate = sampleRate;
    }
    
    /** Calculate the modulation samples for the length of the buffer.
    */
    virtual void prepareModulationBuffer(int numSamples) = 0;
    
    /** Get a modulation sample from the buffer.
    */
    virtual type getModulationSample(int sampleIndex)
    {
        return samples[sampleIndex];
    }
    
    virtual ~ModulationSource() {};
    
    /** Set a modulation sample in the buffer.
    */
    void setModulationSample(int sampleIndex, type value)
    {
        samples[sampleIndex] = value;
    }
    
private:
    std::vector<type> samples;
    double sampleRate = 1.0;
};

} // namespace DSPTools

#endif // DSPTOOLS_MODULATION_SOURCE_HEADER_INCLUDED
