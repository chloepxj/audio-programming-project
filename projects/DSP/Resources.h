#pragma once


namespace DSP {

// typedef uint8_t ResourceId;

extern const float lut_sine[];
extern const float lut_4_decades[];
extern const float lut_svf_shift[];
extern const float lut_stiffness[];
extern const float lut_fm_frequency_quantizer[];



#define LUT_SINE 0
#define LUT_SINE_SIZE 5121
#define LUT_4_DECADES 1
#define LUT_4_DECADES_SIZE 257
#define LUT_SVF_SHIFT 2
#define LUT_SVF_SHIFT_SIZE 257
#define LUT_STIFFNESS 3
#define LUT_STIFFNESS_SIZE 257
#define LUT_FM_FREQUENCY_QUANTIZER 4
#define LUT_FM_FREQUENCY_QUANTIZER_SIZE 129



}