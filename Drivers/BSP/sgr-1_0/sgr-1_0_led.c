/*
 * sgr-1_0_led.c
 *
 *  Created on: Mar 29, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#include "sgr-1_0_led.h"

static GPIO_TypeDef* led_port[kLedAmount] = { LED1_LED_PORT, LED2_LED_PORT};
static const int32_t led_pin[kLedAmount] = { LED1_PIN, LED2_PIN };

int32_t BSP_LED_Init(Led_t led) {
	GPIO_InitTypeDef gpio_init_structure;

	// Enable the GPIO_LED Clock
	switch (led) {
	case kLed1:
		LED1_GPIO_CLK_ENABLE();
		break;
	case kLed2:
		LED2_GPIO_CLK_ENABLE();
		break;
	default:
		return BSP_ERROR_WRONG_PARAM;
	}

	// configure the GPIO_LED pin
	gpio_init_structure.Pin = led_pin[led];
	gpio_init_structure.Mode = GPIO_MODE_OUTPUT_OD;
	gpio_init_structure.Pull = GPIO_NOPULL;
	gpio_init_structure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(led_port[led], &gpio_init_structure);

	HAL_GPIO_WritePin(led_port[led], led_pin[led], GPIO_PIN_SET);

	return BSP_ERROR_NONE;
}

int32_t BSP_LED_DeInit(Led_t led) {
	if ((led < kLed1) || led >= kLedAmount) {
		return BSP_ERROR_WRONG_PARAM;
	}

	GPIO_InitTypeDef gpio_init_structure;

	// DeInit the GPIO_LED pin
	gpio_init_structure.Pin = led_pin[led];

	// Turn off LED
	HAL_GPIO_WritePin(led_port[led], led_pin[led], GPIO_PIN_SET);
	HAL_GPIO_DeInit(led_port[led], gpio_init_structure.Pin);

	return BSP_ERROR_NONE;
}

int32_t BSP_LED_On(Led_t led) {
	if ((led < kLed1) || led >= kLedAmount) {
		return BSP_ERROR_WRONG_PARAM;
	}

	HAL_GPIO_WritePin(led_port[led], led_pin[led], GPIO_PIN_RESET);

	return BSP_ERROR_NONE;
}

int32_t BSP_LED_Off(Led_t led) {
	if ((led < kLed1) || led >= kLedAmount) {
		return BSP_ERROR_WRONG_PARAM;
	}
	HAL_GPIO_WritePin(led_port[led], led_pin[led], GPIO_PIN_SET);

	return BSP_ERROR_NONE;
}

int32_t BSP_LED_Toggle(Led_t led) {
	if ((led < kLed1) || led >= kLedAmount) {
		return BSP_ERROR_WRONG_PARAM;
	}

	HAL_GPIO_TogglePin(led_port[led], led_pin[led]);

	return BSP_ERROR_NONE;
}

int32_t BSP_LED_GetState(Led_t led) {
	if ((led < kLed1) || led >= kLedAmount) {
		return BSP_ERROR_WRONG_PARAM;
	}

	return ((HAL_GPIO_ReadPin(led_port[led], led_pin[led]) == GPIO_PIN_SET) ? BSP_LED_OFF : BSP_LED_ON);
}
