/*
 * dsp.c
 *
 *  Created on: Oct 9, 2024
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#include "dsp.h"

int32_t dsp_filter_init(DspFilterTypeDef* filter_struct, float32_t sample_rate, uint16_t nat_freq, uint8_t q_factor) {
	if (filter_struct == NULL) {
		return LIB_DSP_ERROR_WRONG_PARAM;
	}

	int32_t ret_val = LIB_DSP_ERROR_NONE;
	float32_t freq = (float32_t) nat_freq;

	if (freq > (sample_rate / 2.0f)) {
		freq = sample_rate / 2.0f;
		ret_val = LIB_DSP_ERROR_HIGH_FILTER_FREQ;
	}

	float32_t w_nat = 2.0f * PI * freq;
	float32_t damping = 1.0f / (0.02f * (float32_t) q_factor);
	float32_t k = 2.0f * sample_rate;

	float32_t b = w_nat * w_nat;
	float32_t a[3] = { 1.0f, 2.0f * damping * w_nat, w_nat * w_nat };
	float32_t norm = (a[0] * k * k) + (a[1] * k) + a[2];

#if (!defined(MCU_MFLOAT_ABI) || (MCU_MFLOAT_ABI > 0U))
	filter_struct->coeffs[0] = -((2.0f * a[2]) - (2.0f * a[0] * k * k)) / norm;
	filter_struct->coeffs[1] = -((a[0] * k * k) - (a[1] * k) + a[2]) / norm;
	filter_struct->coeffs[2] = b / norm;
	filter_struct->coeffs[3] = (2.0f * b) / norm;
	filter_struct->coeffs[4] = b / norm;

	for (uint8_t i = 0U; i < LIB_DSP_FILTER_STATES; ++i) {
		filter_struct->state[i] = 0.0f;
	}

	arm_biquad_cascade_df1_init_f32(&filter_struct->dsp_filter, 1U, &filter_struct->coeffs[0], &filter_struct->state[0]);
#else
	uint8_t shift = 0U;
	float32_t coeffs[LIB_DSP_FILTER_COEFFS] = { -((2.0f * a[2]) - (2.0f * a[0] * k * k)) / norm,
	                                            -((a[0] * k * k) - (a[1] * k) + a[2]) / norm,
																							b / norm,
																							(2.0f * b) / norm,
																							b / norm };

	for (uint8_t i = 0U, j = 0U; i < LIB_DSP_FILTER_COEFFS; ++i) {
		while ((coeffs[i] > 1.0f) || (coeffs[i] <= -1.0f)) {
			++shift;
			for (j = 0U; j < LIB_DSP_FILTER_COEFFS; ++j) {
				coeffs[j] /= 2.0f;
			}
		}
	}

	arm_float_to_q31(&coeffs[0], &filter_struct->coeffs[0], LIB_DSP_FILTER_COEFFS);

	for (uint8_t i = 0U; i < LIB_DSP_FILTER_STATES; ++i) {
		filter_struct->state[i] = 0;
	}

	arm_biquad_cascade_df1_init_q31(&filter_struct->dsp_filter, 1U, &filter_struct->coeffs[0], &filter_struct->state[0],
		                              shift);
#endif  // MCU_MFLOAT_ABI

	return ret_val;
}

int32_t dsp_filter_process(DspFilterTypeDef* filter_struct, int32_t input, int32_t* p_output) {
	if ((filter_struct == NULL) || (p_output == NULL)) {
		return LIB_DSP_ERROR_WRONG_PARAM;
	}

#if (!defined(MCU_MFLOAT_ABI) || (MCU_MFLOAT_ABI > 0U))
	float32_t aux[2] = { 0.0f };
	arm_q31_to_float(&input, &aux[0], 1U);
	arm_biquad_cascade_df1_f32(&filter_struct->dsp_filter, &aux[0], &aux[1], 1U);
	arm_float_to_q31(&aux[1], p_output, 1U);
#else
	arm_biquad_cascade_df1_q31(&filter_struct->dsp_filter, &input, p_output, 1U);
#endif  // MCU_MFLOAT_ABI
	return LIB_DSP_ERROR_NONE;
}
