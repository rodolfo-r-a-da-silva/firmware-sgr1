/*
 * sgr-1_0_adc.h
 *
 *  Created on: Oct 21, 2024
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_SGR_1_0_SGR_1_0_ADC_H_
#define BSP_SGR_1_0_SGR_1_0_ADC_H_

#include <stdint.h>

#include "sgr-1_0_conf.h"
#include "sgr-1_0_errno.h"

#define BSP_ADC_SCALE					3300U
#define BSP_ADC1_BITS					12U
#define BSP_ADC1_SAMPLE_RATE	(12000000.0 / ((10.0 * (12.5 + 41.5)) + 12.5 + 239.5))
#define BSP_ADC_CHANNELS			11U

#define BSP_ADC_VERIFY_THRESHOLD_HIGH	((3100U * 4095U) / 3300U)
#define BSP_ADC_VERIFY_THRESHOLD_LOW	((200U * 4095U) / 3300U)

#define BSP_ADC_DATA_NOT_READY	0
#define BSP_ADC_DATA_READY			1

typedef enum {
	kAdc1,
	kAdcAmount
} AdcTypeDef;

int32_t BSP_ADC_Init(AdcTypeDef instance);
int32_t BSP_ADC_DeInit(AdcTypeDef instance);
int32_t BSP_ADC_GetData(uint8_t channel, int32_t* p_data);
int32_t BSP_ADC_GetFullScale(uint8_t channel, int32_t* p_fullscale);
int32_t BSP_ADC_GetOuputDataRate(uint8_t channel, float* p_odr);

#endif /* BSP_SGR_1_0_SGR_1_0_ADC_H_ */
