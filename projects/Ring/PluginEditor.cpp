#include "PluginProcessor.h"
#include "PluginEditor.h"

MainProcessorEditor::MainProcessorEditor(MainProcessor& p) :
    AudioProcessorEditor(&p), audioProcessor(p),
    LPFParameterEditor(audioProcessor.getParameterManager(), ParamHeight,
                        { Param::ID::Enabled, Param::ID::Drive, Param::ID::CutoffFreq, Param::ID::Resonance, Param::ID::Mode }),
    ResonatorParameterEditor(audioProcessor.getParameterManager(), ParamHeight, 
                        {Param::ID::Frequency, Param::ID::Structure, Param::ID::Brightness, Param::ID::Damping, Param::ID::PostGain})
    
{
    // setLookAndFeel(&laf);
    addAndMakeVisible(LPFParameterEditor);
    addAndMakeVisible(ResonatorParameterEditor);

    setSize(NumOfBands * BandWidth, ParamsPerBand * ParamHeight);

}

MainProcessorEditor::~MainProcessorEditor()
{
}

void MainProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void MainProcessorEditor::resized()
{
    auto localBounds { getLocalBounds() };
    LPFParameterEditor.setBounds(localBounds.removeFromLeft(BandWidth));
    ResonatorParameterEditor.setBounds(localBounds);
    
}
