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

#ifndef DSPTOOLS_AUDIO_BUFFER_INFO_HEADER_INCLUDED
#define DSPTOOLS_AUDIO_BUFFER_INFO_HEADER_INCLUDED

#include <cassert>

namespace DSPTools {

template <typename type>
class AudioBufferInfo
{
public:
    AudioBufferInfo () {}
    ~AudioBufferInfo () {}
    
    /** Adds a channel of audio samples to the buffer info object.
    */
    void appendChannel(int numSamples, type* newData, int channelIndex)
    {
        assert(numSamples > 0);
        assert(channelIndex >= 0);
        if (channelIndex == 0)
        {
            data.clear();
        }
        data.push_back(newData);
        this->numSamples = numSamples;
    }
    
    /** Returns the number of samples in the buffer info object.
    */
    int getNumSamples()
    {
        return numSamples;
    }
    
    /** Returns the number of channels in the buffer info object.
    */
    unsigned long getNumChannels()
    {
        return data.size();
    }
    
    /** Returns a pointer to the audio data of the chosen channel.
    */
    type* getChannelData(int channel)
    {
        return data[channel];
    }
    
private:
    std::vector<type*> data;
    int numSamples = 0;
};

} //  namespace DSPTools

#endif // DSPTOOLS_AUDIO_BUFFER_INFO_HEADER_INCLUDED
