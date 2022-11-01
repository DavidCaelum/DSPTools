# DSPTools
This is a collection of classes that I find useful for creating audio plugins. Although I use JUCE, these classes are designed so that JUCE is not required.

DSPTools is header-only. `#include` the files you need or `#include` [DSPTools.h](./include/DSPTools.h) to use any DSPTools class.

A simple example of a JUCE project that uses these classes can be found in [JUCE Example](./examples/JUCEExample). Currently this has no useable UI but its audio parameters can be controlled from a host DAW.

-----------------------------------------------------------------------
### Features

- A parameter [modulation](./include/Modulation) system with a *soon to appear* range of modulator types. Currently this is limited to a [basic waveform modulator with sine, triangle, square and sawtooth shapes.](./include/Modulation/WaveModulator.h)

- A range of [audio effects](./include/Processors) such as a [compressor](./include/Processors/Compressor.h) where all parameters ***CAN*** be modulated using the [ModulationParameter](./include/Modulation/ModulationParameter.h) class from the above modulation system.

- A variety of [maths](./include/Utilities/Maths.h) functions that I find useful.

- Some [waveshapers](./include/Utilities/Waveshapers.h) that will eventually make it into their own audio effect. These are currently limited and will be expanded.

- An [AudioBufferInfo](./include/Utilities/AudioBufferInfo.h) class to pass around and process audio data.

- Other useful [utilities.](./include/Utilities)

- [Oscillators and audio sources.](./include/AudioSources) Please note that currently only a basic oscillator is available that will produce aliasing. A minBLEP derived class is on its way.