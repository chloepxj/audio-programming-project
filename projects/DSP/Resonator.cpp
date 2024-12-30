#include "Resonator.h"
// #include <CarbonCore/Gestalt.h>
#include <algorithm>
#include <cmath>


namespace DSP {

Resonator::Resonator():
    freqRamp(0.02f),
    strucRamp(0.02f),
    brightness(0.02f),
    posRamp(0.02f),
    dampRamp(0.02f)
{
}

Resonator::~Resonator()
{
}

void Resonator::prepare(double newSampleRate, unsigned int numChannels)
{
    for (int i = 0; i < kMaxModes; ++i) 
    {
        //SVFs[i].prepare(sampleRate);
        svf[i].Init();
    }
    
    // prepare ramps
    freqRamp.prepare(newSampleRate, true, frequency);
    strucRamp.prepare(newSampleRate, true, structure);
    brightRamp.prepare(newSampleRate, true, brightness);
    posRamp.prepare(newSampleRate, true, position);
    dampRamp.prepare(newSampleRate, true, damping);

    // setFrequency(220.0f);
    // setStructure(0.25f);
    // setBrightness(0.5f);
    // setDamping(0.3f);
    // setPosition(0.999f);
    previous_position = 0.0f;

    setResolution(kMaxModes);


    phaseState[0] = 0.f;
    phaseState[1] = static_cast<float>(M_PI / 2);
    phaseInc = static_cast<float>(2.0 * M_PI) * position;

    // num_modes = ComputeFilters();
}



void Resonator::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    int num_modes = ComputeFilters();
    
    // ParameterInterpolator position_ (&previous_position, position, static_cast<size_t>(numSamples));
    
    numChannels = std::min(numChannels, 2u); 

    for (unsigned int n = 0; n < numSamples; ++n)
    {
        float lfo[2] {0.f, 0.f};
        lfo[0] = std::pow(0.5f + 0.5f * std::sin(phaseState[0]), 2.f);
        lfo[1] = std::pow(0.5f + 0.5f * std::sin(phaseState[1]), 2.f);

        phaseState[0] = std::fmod(phaseState[0] + phaseInc, static_cast<float>(2 * M_PI));
        phaseState[1] = std::fmod(phaseState[1] + phaseInc, static_cast<float>(2 * M_PI));

        float x[2] {input[0][n], input[1][n]};
        float y[2] {0.f, 0.f};        

        float odd;
        float even;    
        // loop thru each modes
        // for the odd mode, ch0 is processed by svf[odd]
        // for the even mode, ch1 is processed by svf[even]
        for (int i = 0; i < num_modes;) //process through each filter 
        {
            odd = svf[i++].Process<FILTER_MODE_BAND_PASS>(x[0]);
            y[0] += odd;

            even = svf[i++].Process<FILTER_MODE_BAND_PASS>(x[1]);
            y[1] += even;
            
        }

        y[0] *= lfo[0];
        y[1] *= lfo[1];

        // Write to output buffers
        for ( unsigned int ch = 0; ch < numChannels; ch++)
        {
            output[ch][n] = y[ch];
        }
    }
}

void Resonator::setFrequency(float freqHz)
{
    frequency = freqHz/sampleRate;
    freqRamp.setTarget(frequency);
}

void Resonator::setStructure(float newStructure)
{
    structure = newStructure;
    // stiffness = Interpolate(lut_stiffness, structure, 256.0f);
    strucRamp.setTarget(structure);    
}

void Resonator::setBrightness(float newBrightness)
{
    brightness = newBrightness;
    brightRamp.setTarget(brightness);
}

void Resonator::setDamping(float newDamping)
{
    damping = newDamping;
    dampRamp.setTarget(damping);
}

void Resonator::setPosition(float newPosition)
{
    position = newPosition;
    posRamp.setTarget(position);
    phaseInc = static_cast<float>(2.0 * M_PI) * position;
}

void Resonator::setResolution(int newResolution)
{
    newResolution -= newResolution & 1; // Must be even!
    resolution = std::min(newResolution, kMaxModes);
}

int Resonator::ComputeFilters()
{

    // structure = strucRamp.applyGain(1, numChannels);
    float stiffness = Interpolate(lut_stiffness, structure, 256.0f);

    float harmonic = frequency;
    float stretch_factor = 1.0f;  // inharmonicity factor B ~ [-0.06, 2]

    // test_q = Interpolate(lut_4_decades, damping, 256.0f);
    float q = 500.0f * Interpolate(lut_4_decades, damping, 256.0f);
    
    //  
    float brightness_attenuation = 1.0f - structure;
    // Reduces the range of brightness when structure is very low, to prevent
    // clipping.
    brightness_attenuation *= brightness_attenuation;
    brightness_attenuation *= brightness_attenuation;
    brightness_attenuation *= brightness_attenuation;

    float brightness_ = brightness * (1.0f - 0.2f * brightness_attenuation);
    float q_loss = brightness_ * (2.0f - brightness_) * 0.85f + 0.15f;
    float q_loss_damping_rate = structure * (2.0f - structure) * 0.1f;
    
    int num_modes = 0;

    for (int i = 0; i < std::min(kMaxModes, resolution); ++i)  //0 ~ 64
    {
        float partial_frequency = harmonic * stretch_factor;

        if (partial_frequency >= 0.49f) 
        {
            partial_frequency = 0.49f;
        } 
        else 
        {
            num_modes = i + 1;
        }
        
        svf[i].set_f_q<FREQUENCY_FAST>(partial_frequency,1.0f + partial_frequency * q);
        // filters[i].(partial_frequency, 1.0f + partial_frequency * q);
        
        stretch_factor += stiffness;

        if (stiffness < 0.0f) 
        {
            // Make sure that the partials do not fold back into negative frequencies.
            stiffness *= 0.93f;
        }
        else
        {
            // This helps adding a few extra partials in the highest frequencies.
            stiffness *= 0.98f;
        }

        // This prevents the highest partials from decaying too fast.
        q_loss += q_loss_damping_rate * (1.0f - q_loss);
        harmonic += frequency;
        q *= q_loss;
    }
    
    return num_modes;

}   

}