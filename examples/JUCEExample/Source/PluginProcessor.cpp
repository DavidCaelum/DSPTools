/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DSPToolsAudioProcessor::DSPToolsAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
,parameters (*this, nullptr, juce::Identifier ("DSPTools"),
{
    std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("gain", 1.0),            // parameter ID
                                            "Gain",            // parameter name
                                            juce::NormalisableRange<float>(-60.0, 6.0, 0.1),              // maximum value
                                            0.0),
    std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("gainM", 1.0),            // parameter ID
                                            "Gain Modulation",            // parameter name
                                            juce::NormalisableRange<float>(-100.0, 100.0, 0.1),              // maximum value
                                            0.0),
    std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("pan", 1.0),            // parameter ID
                                            "Pan",            // parameter name
                                            juce::NormalisableRange<float>(-100.0, 100.0, 0.1),              // maximum value
                                            0.0),
    std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("panM", 1.0),            // parameter ID
                                            "Pan Modulation",            // parameter name
                                            juce::NormalisableRange<float>(-100.0, 100.0, 0.1),              // maximum value
                                            0.0),
    std::make_unique<juce::AudioParameterFloat> (juce::ParameterID("modF", 1.0),            // parameter ID
                                            "Modulation Frequency",            // parameter name
                                            juce::NormalisableRange<float>(0.1, 20.0, 0.1),              // maximum value
                                            2.0),
    std::make_unique<juce::AudioParameterChoice>(juce::ParameterID("modS", 1.0),
                                            "Modulation Shape", juce::StringArray("Sine", "Triangle", "Square", "Saw"),
                                            0)
})
#endif
{
    gainParameter = parameters.getRawParameterValue("gain");
    gainModulationParameter = parameters.getRawParameterValue("gainM");
    
    modulationFrequencyParameter = parameters.getRawParameterValue("modF");
    modulationShapeParameter = parameters.getRawParameterValue("modS");
    
    panParameter = parameters.getRawParameterValue("pan");
    panModulationParameter = parameters.getRawParameterValue("panM");
}

DSPToolsAudioProcessor::~DSPToolsAudioProcessor()
{
}

//==============================================================================
const juce::String DSPToolsAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DSPToolsAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DSPToolsAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DSPToolsAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DSPToolsAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DSPToolsAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DSPToolsAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DSPToolsAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DSPToolsAudioProcessor::getProgramName (int index)
{
    return {};
}

void DSPToolsAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DSPToolsAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    waveModulator = std::make_unique<DSPTools::WaveModulator<float>>();
    waveModulator->setup(samplesPerBlock, sampleRate);
    
    gain.setup(sampleRate, samplesPerBlock, getMainBusNumOutputChannels());
    gain.setDecibelRange(-60.0, 6.0);
    gain.setGainModulationSource(waveModulator);
    
    pan.setup(sampleRate, samplesPerBlock, getMainBusNumOutputChannels());
    pan.setPannerModulationSource(waveModulator);
}

void DSPToolsAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DSPToolsAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())
    {
        // Mono-to-stereo OR stereo-to-stereo
        if ((layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono()) ||
                (layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo()))
            return true;
    }
    return false;
}
#endif

void DSPToolsAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();

    DSPTools::AudioBufferInfo<float> bufferInfo;
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        bufferInfo.appendChannel(buffer.getNumSamples(), buffer.getWritePointer(channel), channel);
    }
    
    waveModulator->prepareModulationBuffer(buffer.getNumSamples());
    waveModulator->setModulationShape(DSPTools::BasicOscillator<float>::Waveshape (modulationShapeParameter->load()));
    waveModulator->setFrequency(modulationFrequencyParameter->load());
    
    gain.setDecibels(gainParameter->load(), gainModulationParameter->load() / 100.0);
    gain.processAudio(bufferInfo);
    
    pan.setPanning(panParameter->load() / 100.0, panModulationParameter->load() / 100.0);
    pan.processAudio(bufferInfo);
}

//==============================================================================
bool DSPToolsAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DSPToolsAudioProcessor::createEditor()
{
    return new DSPToolsAudioProcessorEditor (*this);
}

//==============================================================================
void DSPToolsAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DSPToolsAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DSPToolsAudioProcessor();
}
