/*
 * sgr-1_0_led.h
 *
 *  Created on: Mar 29, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_SGR_1_0_SGR_1_0_LED_H_
#define BSP_SGR_1_0_SGR_1_0_LED_H_

#include <stdint.h>

#include "led.h"
#include "sgr-1_0_conf.h"
#include "sgr-1_0_errno.h"

#define LED1_LED_PORT						((GPIO_TypeDef*) GPIOB)
#define LED1_PIN								((int32_t) GPIO_PIN_13)
#define LED1_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()
#define LED2_LED_PORT						((GPIO_TypeDef*) GPIOB)
#define LED2_PIN								((int32_t) GPIO_PIN_12)
#define LED2_GPIO_CLK_ENABLE()	__HAL_RCC_GPIOB_CLK_ENABLE()

#endif /* BSP_SGR_1_0_SGR_1_0_LED_H_ */
