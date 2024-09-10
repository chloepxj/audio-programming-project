#pragma once

#include "algorithm"
#include "StateVariableFilter.h"
#include "DelayLine.h"
#include "dsp.h"
#include "Filter.h"
#include "ParameterInterpolator.h"
#include "CosineOscillator.h"


namespace DSP {

const int kMaxModes = 64;

class Resonator {
public:

    Resonator();
    ~Resonator();
    
    Resonator(const Resonator&) = delete;
    Resonator(Resonator&&) = delete;
    const Resonator& operator=(const Resonator&) = delete;
    const Resonator& operator=(Resonator&&) = delete;

    // Update new sample rate
    void prepare(double sampleRate);

    // Process resonator output for a buffer
    void process(const float* in, float* out, float* aux, size_t size);
    void process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples);

    // Process a single sample of the resonator
    float process();

    void setFrequency(float freqHz);
    void setStructure(float newStructure);
    void setBrightness(float newBrightness);
    void setDamping(float newDamping);
    void setPosition(float newPosition);
    void setResolution(int newResolution);
    

private:

    double sampleRate { 48000.0 };

    float frequency { 1.f };
    float structure { 0.25f };
    float brightness { 0.5f };
    float position { 0.999f };
    float previous_position { 0.0f };
    float damping { 0.3f };
    int resolution {kMaxModes};


    int ComputeFilters();

    // StateVariableFilter array for each resonator mode
    // std::vector<DSP::StateVariableFilter> filters;  
    DSP::Svf svf[kMaxModes];



};





}