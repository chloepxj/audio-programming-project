#include "PluginProcessor.h"
#include "JuceHeader.h"
#include "Oscillator.h"
#include "PluginEditor.h"
#include "mrta_utils/Source/Parameter/ParameterInfo.h"
#include "mrta_utils/Source/Parameter/ParameterManager.h"
#include <vector>


static const std::vector<mrta::ParameterInfo> ParameterInfos
{             
    // { Param::ID::AttRelTime,    Param::Name::AttRelTime,  "ms", 100.f, Param::Ranges::AttRelTimeMin, Param::Ranges::AttRelTimeMax, Param::Ranges::AttRelTimeInc, Param::Ranges::AttRelTimeSkw },
    // { Param::ID::WaveType,      Param::Name::WaveType,    Param::Ranges::WaveType, 0 },
    // //
    // { Param::ID::Enabled,       Param::Name::Enabled,     "Off", "On",   true },
              // ID,                        Name,            unit,   defualt,   min,     max,        inc,      skew
    // { Param::ID::Drive,         Param::Name::Drive,       "",     1.f,      1.f,     10.f,       0.1f,     1.f },
    // { Param::ID::CutoffFreq,    Param::Name::CutoffFreq,  "Hz",   1000.f,   20.f,    20000.f,    1.f,      0.3f},
    // { Param::ID::Resonance,     Param::Name::Resonance,   "",     0.f,      0.f,     1.f,        0.001f,   1.f },
    // { Param::ID::Mode,          Param::Name::Mode,        { "LPF12", "HPF12", "BPF12", "LPF24", "HPF24", "BPF24" }, 3 },

    { Param::ID::Frequency,     Param::Name::Frequency,   "Hz",   100.f,    20.f,    20000.f,    1.f,      0.3f},
    { Param::ID::Structure,     Param::Name::Structure,   "",     0.25f,    0.f,     0.9995f,    0.001f,   1.f },
    { Param::ID::Brightness,    Param::Name::Brightness,  "",     0.5f,     0.f,     1.f,        0.001f,   1.f },
    { Param::ID::Damping,       Param::Name::Damping,     "",     0.3f,     0.f,     1.f,        0.001f,   1.f },
    { Param::ID::Position,      Param::Name::Position,    "",     0.999f,   0.f,     1.f,        0.001f,   1.f },
    { Param::ID::PostGain,      Param::Name::PostGain,    "dB",   0.0f,     -60.f,   12.f,       0.1f,     3.8018f },
};


MainProcessor::MainProcessor() :
    parameterManager(*this, ProjectInfo::projectName, ParameterInfos)
{
    // voice = new DSP::SynthVoice();
    // synth.addSound(new DSP::SynthSound());
    // synth.addVoice(voice);

    // parameterManager.registerParameterCallback(Param::ID::AttRelTime,
    // [this] (float value, bool /*force*/)
    // {
    //     voice->setAttRelTime(value);
    // });

    // parameterManager.registerParameterCallback(Param::ID::WaveType,
    // [this] (float value, bool /*force*/)
    // {
    //     DSP::Oscillator::OscType type = static_cast<DSP::Oscillator::OscType>(std::rint(value));
    //     voice->setWaveType(type);
    // });

    // parameterManager.registerParameterCallback(Param::ID::Enabled,
    // [this] (float value, bool /*forced*/)
    // {
    //     filter.setEnabled(value > 0.5f);
    // });

    // parameterManager.registerParameterCallback(Param::ID::Drive,
    // [this] (float value, bool /*forced*/)
    // {
    //     filter.setDrive(value);
    //     // DBG("drive: " + juce::String{value});
    // });

    parameterManager.registerParameterCallback(Param::ID::CutoffFreq,
    [this] (float value, bool /*forced*/)
    {
        filter.setCutoffFrequencyHz(value);
    });

    parameterManager.registerParameterCallback(Param::ID::Resonance,
    [this] (float value, bool /*forced*/)
    {
        filter.setResonance(value);
    });

    parameterManager.registerParameterCallback(Param::ID::Mode,
    [this] (float value, bool /*forced*/)
    {
        filter.setMode(static_cast<juce::dsp::LadderFilter<float>::Mode>(std::floor(value)));
    });


    //
    parameterManager.registerParameterCallback(Param::ID::Frequency,
    [this] (float value, bool /*forced*/)
    {
        resonator.setFrequency(value);
        // DBG("num_modes: " + ::juce::String(resonator.num_modes));
    });

    parameterManager.registerParameterCallback(Param::ID::Structure,
    [this] (float value, bool /*forced*/)
    {
        resonator.setStructure(value);
        // float num_mode = resonator.ComputeFilters();
        // DBG("num_modes: " + ::juce::String(num_mode)); 

    });

    parameterManager.registerParameterCallback(Param::ID::Brightness,
    [this] (float value, bool /*forced*/)
    {
        resonator.setBrightness(value);
    });

    parameterManager.registerParameterCallback(Param::ID::Damping,
    [this] (float value, bool /*forced*/)
    {
        resonator.setDamping(value);
        // DBG("test_q: " + juce::String(resonator.test_q)); GOOD
    });

    parameterManager.registerParameterCallback(Param::ID::Position,
    [this] (float value, bool /*forced*/)
    {
        resonator.setPosition(value);
    });

    parameterManager.registerParameterCallback(Param::ID::PostGain,
    [this] (float value, bool forced)
    {
        // DBG(Param::Name::PostGain + ": " + juce::String { value });
        float dbValue { 0.f };
        if (value > -60.f)
            dbValue = std::pow(10.f, value * 0.05f);

        if (forced)
            outputGain.setCurrentAndTargetValue(dbValue);
        else
            outputGain.setTargetValue(dbValue);
    });
}

MainProcessor::~MainProcessor()
{
}

void MainProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::uint32 numChannels { static_cast<juce::uint32>(std::max(getMainBusNumInputChannels(), getMainBusNumOutputChannels())) };
    // synth.setCurrentPlaybackSampleRate(sampleRate);
    
    filter.prepare({ sampleRate, static_cast<juce::uint32>(samplesPerBlock), numChannels });
    // set filter as LFP
    filter.setEnabled(true);
    filter.setDrive(1.f);
    filter.setCutoffFrequencyHz(2000);
    filter.setResonance(0);
    filter.setMode(static_cast<juce::dsp::LadderFilter<float>::Mode>(juce::dsp::LadderFilterMode::LPF24));
    
    resonator.prepare(sampleRate);
    outputGain.reset(sampleRate, 0.01f);
    parameterManager.updateParameters(true);
}

void MainProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    parameterManager.updateParameters();
    // float midiFreq = DSP::convertMidiNoteToFreq(int MidiMessage);

    const unsigned int numChannels{ static_cast<unsigned int>(buffer.getNumChannels()) };
    const unsigned int numSamples{ static_cast<unsigned int>(buffer.getNumSamples()) };

    // synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());


    {
        juce::dsp::AudioBlock<float> audioBlock(buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
        juce::dsp::ProcessContextReplacing<float> ctx(audioBlock);
        filter.process(ctx);
    }
    
    resonator.process(buffer.getArrayOfWritePointers(), buffer.getArrayOfReadPointers(), numChannels, numSamples);


    outputGain.applyGain(buffer, buffer.getNumSamples());
}

void MainProcessor::releaseResources()
{
    filter.reset();
}

void MainProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    parameterManager.getStateInformation(destData);
}

void MainProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    parameterManager.setStateInformation(data, sizeInBytes);
}

juce::AudioProcessorEditor* MainProcessor::createEditor()
{
    return new MainProcessorEditor(*this);
}



//==============================================================================
const juce::String MainProcessor::getName() const { return JucePlugin_Name; }
bool MainProcessor::acceptsMidi() const { return true; }
bool MainProcessor::producesMidi() const { return false; }
bool MainProcessor::isMidiEffect() const { return false; }
double MainProcessor::getTailLengthSeconds() const { return 0.0; }
int MainProcessor::getNumPrograms() { return 1; }
int MainProcessor::getCurrentProgram() { return 0; }
void MainProcessor::setCurrentProgram (int) { }
const juce::String MainProcessor::getProgramName(int) { return {}; }
void MainProcessor::changeProgramName(int, const juce::String&) { }
bool MainProcessor::hasEditor() const { return true; }
//==============================================================================

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MainProcessor();
}
