/*
 * sgr-1_0_conf.h
 *
 *  Created on: Nov 6, 2024
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_SGR_1_0_SGR_1_0_CONF_H_
#define BSP_SGR_1_0_SGR_1_0_CONF_H_

#include <stm32f1xx_hal.h>

#define BSP_SGR_1_0_ADC_ENABLED
#define BSP_SGR_1_0_CAN_ENABLED
#define BSP_SGR_1_0_I2C_ENABLED

#define BSP_MODULE_EEPROM_ENABLED
#define BSP_MODULE_LED_ENABLED
#define BSP_MODULE_WBRIDGE_ENABLED
#define BSP_MODULE_TEMPERATURE_ENABLED

#define BSP_EEPROM_WB_MEM_START 0U
#define BSP_EEPROM_APP_MEM_START ((BSP_WBRIDGE_CHANNELS * 4U) + BSP_EEPROM_WB_MEM_START)

#define BSP_WBRIDGE_CHANNELS 10  // Number of available wheatstone bridge channels

#ifdef BSP_SGR_1_0_ADC_ENABLED
#include "sgr-1_0_adc.h"
#endif  // BSP_SGR_1_0_ADC_ENABLED

#ifdef BSP_SGR_1_0_CAN_ENABLED
#include "sgr-1_0_can.h"
#endif  // BSP_SGR_1_0_CAN_ENABLED

#ifdef BSP_SGR_1_0_I2C_ENABLED
#include "sgr-1_0_i2c.h"
#endif  // BSP_SGR_1_0_I2C_ENABLED

#ifdef BSP_MODULE_EEPROM_ENABLED
#include "sgr-1_0_eeprom.h"
#endif  // BSP_MODULE_EEPROM_ENABLED

#ifdef BSP_MODULE_LED_ENABLED
#include "sgr-1_0_led.h"
#endif  // BSP_MODULE_LED_ENABLED

#ifdef BSP_MODULE_TEMPERATURE_ENABLED
#include "sgr-1_0_temperature.h"
#endif  // BSP_MODULE_TEMPERATURE_ENABLED

#ifdef BSP_MODULE_WBRIDGE_ENABLED
#include "sgr-1_0_wheatstone_bridge.h"
#endif  // BSP_MODULE_WBRIDGE_ENABLED

#endif /* BSP_SGR_1_0_SGR_1_0_CONF_H_ */
