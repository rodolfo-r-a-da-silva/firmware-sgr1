/*
 * dsp.h
 *
 *  Created on: Oct 9, 2024
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef LIB_DSP_H_
#define LIB_DSP_H_

#include "arm_math.h"

#define LIB_DSP_ERROR_NONE	0
#define LIB_DSP_ERROR_WRONG_PARAM	-1
#define LIB_DSP_ERROR_HIGH_FILTER_FREQ  -2

#define LIB_DSP_FILTER_STAGES 1U
#define LIB_DSP_FILTER_COEFFS (LIB_DSP_FILTER_STAGES * 5U)
#define LIB_DSP_FILTER_STATES (LIB_DSP_FILTER_STAGES * 4U)

typedef struct {
#if (!defined(MCU_MFLOAT_ABI) || (MCU_MFLOAT_ABI > 0U))
  float32_t coeffs[LIB_DSP_FILTER_COEFFS];
	float32_t state[LIB_DSP_FILTER_STATES];
	arm_biquad_casd_df1_inst_f32 dsp_filter;
#else
  q31_t coeffs[LIB_DSP_FILTER_COEFFS];
  q31_t state[LIB_DSP_FILTER_STATES];
  arm_biquad_casd_df1_inst_q31 dsp_filter;
#endif  // MCU_MFLOAT_ABI
} DspFilterTypeDef;

int32_t dsp_filter_init(DspFilterTypeDef* filter_struct, float32_t sample_rate, uint16_t nat_freq, uint8_t q_factor);
int32_t dsp_filter_process(DspFilterTypeDef* filter_struct, int32_t input, int32_t* p_output);

#endif /* LIB_DSP_H_ */
