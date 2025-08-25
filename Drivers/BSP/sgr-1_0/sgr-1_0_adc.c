/*
 * sgr-1_0_adc.c
 *
 *  Created on: Nov 6, 2024
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#include "sgr-1_0_adc.h"

static void SetInstances();

static uint32_t adc_buffer[BSP_ADC_CHANNELS] = { 0U };
static uint32_t adc_accumulator[BSP_ADC_CHANNELS] = { 0U };
static uint32_t adc_current_conv[BSP_ADC_CHANNELS] = { 0U };
static int32_t adc_result[BSP_ADC_CHANNELS] = { 0U };
static int32_t adc_result_rdy[BSP_ADC_CHANNELS] = { 0U };

static const uint8_t adc_ovs_bits[BSP_ADC_CHANNELS] = { 3U, 3U, 3U, 3U, 3U, 3U, 3U, 3U, 3U, 3U, 0U };
static uint32_t adc_ovs_iter[BSP_ADC_CHANNELS] = { 0U };

static ADC_HandleTypeDef* hadc[kAdcAmount];
static uint8_t init_counter_adc[kAdcAmount] = { 0U };

extern ADC_HandleTypeDef hadc1;

int32_t BSP_ADC_Init(AdcTypeDef instance) {
	if (instance >= kAdcAmount) {
		return BSP_ERROR_WRONG_PARAM;
	}

	int32_t ret_val = BSP_ERROR_NONE;
	uint32_t state = HAL_ADC_STATE_RESET;

	if ((init_counter_adc[instance]++ == 0U) && (HAL_ADC_GetState(hadc[instance]) == HAL_ADC_STATE_RESET)) {
		SetInstances();

		if (HAL_ADCEx_Calibration_Start(hadc[instance]) != HAL_OK) {
			ret_val = BSP_ERROR_PERIPH_FAILURE;
		}

		for (uint8_t i = 0U; i < BSP_ADC_CHANNELS; ++i) {
			adc_ovs_iter[i] = (4U << (2U * (adc_ovs_bits[i] - 1U)));
		}

		state = HAL_ADC_GetState(hadc[instance]);

		if (state == HAL_ADC_STATE_READY) {
		  ret_val = (HAL_ADC_Start_DMA(hadc[instance], &adc_buffer[0], BSP_ADC_CHANNELS) == HAL_OK)
		      ? BSP_ERROR_NONE : BSP_ERROR_PERIPH_FAILURE;

    } else if (state == HAL_ADC_STATE_RESET) {
      ret_val = BSP_ERROR_NO_INIT;
    } else {
      ret_val = BSP_ERROR_PERIPH_FAILURE;
    }

		if (ret_val < BSP_ERROR_NONE) {
      --init_counter_adc[instance];
    }
	}

	return ret_val;
}

int32_t BSP_ADC_DeInit(AdcTypeDef instance) {
  if (instance >= kAdcAmount) {
    return BSP_ERROR_WRONG_PARAM;
  }

  int32_t ret_val = BSP_ERROR_NONE;

  if (init_counter_adc[instance] > 0U) {
    if (--init_counter_adc[instance] == 0U) {
      ret_val = (HAL_ADC_Stop_DMA(hadc[instance]) == HAL_OK) ? BSP_ERROR_NONE : BSP_ERROR_PERIPH_FAILURE;

      if (ret_val >= BSP_ERROR_NONE) {
        ret_val = (HAL_ADC_DeInit(hadc[instance]) == HAL_OK) ? BSP_ERROR_NONE : BSP_ERROR_PERIPH_FAILURE;
      }
    }
  }

  if (ret_val < BSP_ERROR_NONE) { ++init_counter_adc[instance]; }

  return ret_val;
}

int32_t BSP_ADC_GetData(uint8_t channel, int32_t* p_data) {
	if ((channel >= BSP_ADC_CHANNELS) || (p_data == NULL)) {
		return BSP_ERROR_WRONG_PARAM;
	} else if (init_counter_adc[kAdc1] == 0U) {
    return BSP_ERROR_NO_INIT;
  } else {
    // Do nothing
  }

	int32_t ret_val = adc_result_rdy[channel];

	adc_result_rdy[channel] = BSP_ADC_DATA_NOT_READY;
	*p_data = adc_result[channel];

	if ((adc_buffer[channel] < BSP_ADC_VERIFY_THRESHOLD_LOW)
      || (adc_buffer[channel] > BSP_ADC_VERIFY_THRESHOLD_HIGH)) {
    ret_val = BSP_ERROR_COMPONENT_FAILURE;
  }

	return ret_val;
}

int32_t BSP_ADC_GetFullScale(uint8_t channel, int32_t* p_fullscale) {
	if ((channel >= BSP_ADC_CHANNELS) || (p_fullscale == NULL)) {
		return BSP_ERROR_WRONG_PARAM;
	}

	*p_fullscale = (1U << (BSP_ADC1_BITS + adc_ovs_bits[channel])) - 1U;

	return BSP_ERROR_NONE;
}

int32_t BSP_ADC_GetOuputDataRate(uint8_t channel, float* p_odr) {
	if ((channel >= BSP_ADC_CHANNELS) || (p_odr == NULL)) {
		return BSP_ERROR_WRONG_PARAM;
	}

	*p_odr = (float) (BSP_ADC1_SAMPLE_RATE / ((double) adc_ovs_iter[channel]));

	return BSP_ERROR_NONE;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	UNUSED(hadc);
	static uint8_t i = 0U;

	for (i = 0U; i < BSP_ADC_CHANNELS; ++i) {
		adc_accumulator[i] += adc_buffer[i];

		if (adc_current_conv[i] == adc_ovs_iter[i]) {
			adc_result[i] = adc_accumulator[i] >> adc_ovs_bits[i];
			adc_accumulator[i] = 0U;
			adc_current_conv[i] = 0U;
			adc_result_rdy[i] = BSP_ADC_DATA_READY;

			if ((adc_buffer[i] < BSP_ADC_VERIFY_THRESHOLD_LOW)
          || (adc_buffer[i] > BSP_ADC_VERIFY_THRESHOLD_HIGH)) {
			  adc_result_rdy[i] = BSP_ERROR_COMPONENT_FAILURE;
			}
		} else {
			++adc_current_conv[i];
		}
	}
}

static void SetInstances() {
	hadc[kAdc1] = &hadc1;
}
