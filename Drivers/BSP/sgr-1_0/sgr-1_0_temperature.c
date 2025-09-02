/*
 * sgr-1_0_temperature.c
 *
 *  Created on: Feb 7, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#include "sgr-1_0_temperature.h"
#include "stm32f1xx_ll_adc.h"

/**
 * @brief Data storage for the temperature sensor instance
 */
static struct {
  int32_t avg_slope;  /**< Sensor T x V slope in uV/°C */
  int32_t typ_calx_volt;  /**< Fixed V in mV for a fixed T value */
  int32_t calx_temp;  /**< Fixed T value in °C * 10 */
  int32_t resolution;  /**< ADC resolution */
} temperature_ctx;

/**
 * @brief Initialize the MCU's internal temperature sensor
 *
 * Initializes the peripheral used for sensing (ADC in this case). Sets sensor
 * characteristics for temperature calculation.
 *
 * @return An error code if negative
 */
int32_t BSP_TEMPERATURE_Init() {
  temperature_ctx.avg_slope = 4300;
  temperature_ctx.typ_calx_volt = 1430;
  temperature_ctx.calx_temp = 250;

	int32_t ret_val = BSP_ADC_Init(kAdc1);

	if (ret_val >= BSP_ERROR_NONE) {
	  ret_val = BSP_ADC_GetFullScale(BSP_TEMPERATURE_ADC_CHANNEL, &temperature_ctx.resolution);
	}

	return ret_val;
}

/**
 * @brief Get the temperature sensor value in °C * 10
 *
 * @param p_tempertature A pointer to the variable that stores the temperature
 * out of this file's scope
 * @return An error code if negative, new data if higher than zero
 */
int32_t BSP_TEMPERATURE_Get(int16_t* p_temperature) {
	if (p_temperature == NULL) {
		return BSP_ERROR_WRONG_PARAM;
	}

	int32_t input = 0U;

	int32_t ret_val = BSP_ADC_GetData(BSP_TEMPERATURE_ADC_CHANNEL, &input);

	*p_temperature = __LL_ADC_CALC_TEMPERATURE_TYP_PARAMS(temperature_ctx.avg_slope,
                                                        temperature_ctx.typ_calx_volt,
                                                        temperature_ctx.calx_temp,
                                                        BSP_ADC_SCALE,
                                                        input,
                                                        temperature_ctx.resolution);

	return ret_val;
}
