#include "Resonator.h"
#include "Oscillator.h"
#include "Resources.h"
#include "Ramp.h"
#include "SynthVoice.h"
#include <string>

namespace DSP {

Resonator::Resonator()
{
}

Resonator::~Resonator()
{
}

void Resonator::prepare(double sampleRate)
{
    for (int i = 0; i < kMaxModes; ++i) 
    {
        //SVFs[i].prepare(sampleRate);
        svf[i].Init();
    }
    
    setFrequency(220.0f);
    setStructure(0.25f);
    setBrightness(0.5f);
    setDamping(0.3f);
    setPosition(0.999f);
    previous_position = 0.0f;
    setResolution(kMaxModes);

    // num_modes = ComputeFilters();
}

void Resonator::process(float* const* output, const float* const* input, unsigned int numChannels, unsigned int numSamples)
{
    int num_modes = ComputeFilters();
    
    ParameterInterpolator position_ (&previous_position, position, static_cast<size_t>(numSamples));
    
    numChannels = std::min(numChannels, 2u);

    for (unsigned int n = 0; n < numSamples; ++n)
    {
        CosineOscillator amplitudes;
        amplitudes.Init<COSINE_OSCILLATOR_APPROXIMATE>(position_.Next());

        float odd;
        float even;
        amplitudes.Start();
        
        // copy one channel from input buffer
        float x = 0.125f * input[0][n];
   
        for (int i = 0; i < num_modes;) //process through each filter 
        {
            odd = amplitudes.Next() * svf[i++].Process<FILTER_MODE_BAND_PASS>(x);
            output[0][n] += odd;

            even = amplitudes.Next() * svf[i++].Process<FILTER_MODE_BAND_PASS>(x);
            output[1][n] += even;

        }

    }
}

void Resonator::setFrequency(float freqHz)
{
    frequency = freqHz/sampleRate;
}

void Resonator::setStructure(float newStructure)
{
    structure = newStructure;
    // stiffness = Interpolate(lut_stiffness, structure, 256.0f);
}

void Resonator::setBrightness(float newBrightness)
{
    brightness = newBrightness;
}

void Resonator::setDamping(float newDamping)
{
    damping = newDamping;
}

void Resonator::setPosition(float newPosition)
{
    position = newPosition;
}

void Resonator::setResolution(int newResolution)
{
    newResolution -= newResolution & 1; // Must be even!
    resolution = std::min(newResolution, kMaxModes);
}

int Resonator::ComputeFilters()
{
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