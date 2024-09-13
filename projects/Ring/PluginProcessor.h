#pragma once

#include <JuceHeader.h>
#include "Resonator.h"
#include "SynthVoice.h"

#include "mrta_utils/Source/Parameter/ParameterManager.h"


namespace Param
{
    namespace ID
    {
        static const juce::String AttRelTime { "att_rel_time" };
        static const juce::String WaveType { "wave_type" };

        static const juce::String Enabled { "enabled" };
        static const juce::String Drive { "drive" };
        static const juce::String CutoffFreq { "cutoff_frequency" };
        static const juce::String Resonance { "resonance" };
        static const juce::String Mode { "mode" };

        static const juce::String Frequency { "frequency" };
        static const juce::String Structure { "structure" };
        static const juce::String Brightness { "brightness" };
        static const juce::String Damping { "damping" };
        static const juce::String Position { "position" };
        static const juce::String PostGain { "post_gain" };
    }

    namespace Name
    {
        static const juce::String AttRelTime { "Att. Rel. Time" };
        static const juce::String WaveType { "Wave Type" };

        static const juce::String Enabled { "Enabled" };
        static const juce::String Drive { "Drive" };
        static const juce::String CutoffFreq { "Cutoff-Frequency" };
        static const juce::String Resonance { "Resonance" };
        static const juce::String Mode { "Mode" };

        static const juce::String Frequency { "Frequency" };
        static const juce::String Structure { "Structure" };
        static const juce::String Brightness { "Brightness" };
        static const juce::String Damping { "Damping" };
        static const juce::String Position { "Position" };
        static const juce::String PostGain { "Post-Gain" };
    }
    namespace Ranges
    {
        static constexpr float AttRelTimeMin { 1.f };
        static constexpr float AttRelTimeMax { 1000.f };
        static constexpr float AttRelTimeInc { 0.1f };
        static constexpr float AttRelTimeSkw { 0.5f };

        static const juce::StringArray WaveType { "Sine", "Tri. Aliased", "Saw Aliased", "Tri. AA", "Saw AA" };
    }
}

class MainProcessor : public juce::AudioProcessor
{
public:
    MainProcessor();
    ~MainProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    mrta::ParameterManager& getParameterManager() { return parameterManager; }

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;
    //==============================================================================

private:
    mrta::ParameterManager parameterManager;
    
    // juce::Synthesiser synth;
    // DSP::SynthVoice* voice { nullptr };   

    // LPF
    juce::dsp::LadderFilter<float> filter; 

    // Resonator
    DSP::Resonator resonator;

    juce::SmoothedValue<float> outputGain;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainProcessor)
};
