/*
 * readings.c
 *
 *  Created on: Nov 5, 2024
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#include "readings.h"

int32_t readings_init(ReadingTypedef* p_readings, WheatstoneBridge_t wbridge_nbr, uint32_t* p_output,
                      uint8_t* p_verify) {
	if ((p_readings == NULL) || (p_output == NULL) || (p_verify == NULL)) {
	  return BSP_ERROR_WRONG_PARAM;
	}

	p_readings->wbridge = wbridge_nbr;
	p_readings->input = 0;
  p_readings->p_output = (int32_t*) p_output;
  p_readings->p_verify = p_verify;

  p_readings->filter_init = 0U;

  return BSP_WBRIDGE_Init(wbridge_nbr);
}

int32_t readings_set_filter(ReadingTypedef* p_readings, uint16_t* p_freq, uint8_t* p_q_factor) {
	if ((p_readings == NULL) || (p_freq == NULL) || (p_q_factor == NULL)) {
	  return BSP_ERROR_WRONG_PARAM;
	}

	int32_t ret_val = BSP_ERROR_NONE;
	float sample_rate = 0.0f;

  ret_val = BSP_WBRIDGE_GetOutputDataRate(p_readings->wbridge, &sample_rate);

  if (ret_val != BSP_ERROR_NONE) {
    return ret_val;
  }

  ret_val = dsp_filter_init(&p_readings->filter, sample_rate, *p_freq, *p_q_factor);

  if (ret_val == LIB_DSP_ERROR_NONE) {
    p_readings->filter_init = 1U;
  }

	return ret_val;
}

int32_t readings_calc(ReadingTypedef* p_readings) {
  if (p_readings == NULL) {
    return BSP_ERROR_WRONG_PARAM;
  }

  int32_t ret_val = BSP_WBRIDGE_GetVoltage(p_readings->wbridge, &p_readings->input);

  if (ret_val == BSP_WBRIDGE_STRAIN_READY) {
    if (p_readings->filter_init > 0U) {
      ret_val = dsp_filter_process(&p_readings->filter, p_readings->input, p_readings->p_output);
    }

    if (ret_val == BSP_ERROR_NONE) {
      // TODO: add new method to calculate strain
    }
	}

  *p_readings->p_verify = (ret_val < BSP_ERROR_NONE) ? 0U : 1U;

	return ret_val;
}
