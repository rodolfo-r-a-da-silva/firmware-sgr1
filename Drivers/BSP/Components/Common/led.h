/*
 * led.h
 *
 *  Created on: Mar 29, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_COMPONENTS_COMMON_LED_H_
#define BSP_COMPONENTS_COMMON_LED_H_

#include <stdint.h>

#define BSP_LED_OFF	0
#define BSP_LED_ON	1

typedef enum {
	kLed1 = 0U,
	kLedCan = kLed1,
	kLed2 = 1U,
	kLedGP = kLed2,
	kLedAmount
} Led_t;

int32_t BSP_LED_Init(Led_t led);
int32_t BSP_LED_DeInit(Led_t led);
int32_t BSP_LED_On(Led_t led);
int32_t BSP_LED_Off(Led_t led);
int32_t BSP_LED_Toggle(Led_t led);
int32_t BSP_LED_GetState(Led_t led);

#endif /* BSP_COMPONENTS_COMMON_LED_H_ */
