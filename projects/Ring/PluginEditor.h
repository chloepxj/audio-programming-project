#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class MainProcessorEditor : public juce::AudioProcessorEditor
{
public:
    MainProcessorEditor(MainProcessor&);
    ~MainProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    static const int BandWidth { 250 };
    static const int ParamHeight { 80 };
    static const int ParamsPerBand { 5 };
    static const int NumOfBands { 2 };

private:
    MainProcessor& audioProcessor;
    // mrta::GenericParameterEditor genericParameterEditor;
    mrta::GenericParameterEditor LPFParameterEditor;
    mrta::GenericParameterEditor ResonatorParameterEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainProcessorEditor)
};
