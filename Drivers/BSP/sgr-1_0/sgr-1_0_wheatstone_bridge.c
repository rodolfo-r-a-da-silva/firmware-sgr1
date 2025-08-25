/*
 * sgr-1_0_wheatstone_bridge.c
 *
 *  Created on: Feb 7, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#include <stdint.h>

#include "eeprom.h"
#include "sgr-1_0_wheatstone_bridge.h"

#define BSP_WBRIDGE_READING_VOLTAGE BSP_ADC_SCALE
#define BSP_WBRIDGE_VCC 5000
#define BSP_WBRIDGE_ADC_START 0

#define BSP_WBRIDGE_MEM_SIZE 4U

static struct {
  int32_t zero_offset;  // ADC raw zero voltage offset value

  uint8_t init_counter;  // Number of times initialized by application layer

  // Auxiliary data to waste no time allocating memory
  int32_t voltage;  // Wheatstone bridge differential voltage in nV/V
  int32_t raw_voltage;  // Raw ADC voltage value
  int32_t raw[2];
  int32_t cal[2];
} wbridge_ctx[BSP_WBRIDGE_CHANNELS] = { 0 };

int32_t BSP_WBRIDGE_Init(WheatstoneBridge_t instance) {
	if (instance >= BSP_WBRIDGE_CHANNELS) {
		return BSP_ERROR_WRONG_PARAM;
	}

	if (wbridge_ctx[instance].init_counter++ > 0U) { return BSP_ERROR_NONE; }

	int32_t ret_val = BSP_ADC_Init(kAdc1);
	int32_t fullscale = 0;

	uint8_t buffer[BSP_WBRIDGE_MEM_SIZE] = { 0U };
	uint32_t address = BSP_EEPROM_WB_MEM_START + (instance * BSP_WBRIDGE_MEM_SIZE);

	double gain = 0.0;

  if (ret_val >= BSP_ERROR_NONE) {
    ret_val = BSP_EEPROM_Init(0U);
  }

  if (ret_val >= BSP_ERROR_NONE) {
    ret_val = BSP_EEPROM_ReadBuffer(0U, &buffer[0], address, BSP_WBRIDGE_MEM_SIZE);
  }

	if (ret_val >= BSP_ERROR_NONE) {
	  wbridge_ctx[instance].zero_offset  = buffer[0] << 8U;
	  wbridge_ctx[instance].zero_offset |= buffer[1];
    gain = ((double) ((buffer[2] << 8U) | buffer[3])) / 10.0;

    ret_val = BSP_ADC_GetFullScale(instance + BSP_WBRIDGE_ADC_START, &fullscale);
	}

	if (ret_val >= BSP_ERROR_NONE) {
	  for (uint8_t i = 0U; i < 2U; ++i) {
	    wbridge_ctx[instance].raw[i] = i * fullscale;
	    wbridge_ctx[instance].cal[i] =
	        (int32_t) (((double) (wbridge_ctx[instance].raw[i] - wbridge_ctx[instance].zero_offset)
	        * BSP_WBRIDGE_READING_VOLTAGE) * 10000000.0 / (((double) BSP_WBRIDGE_VCC) * ((double) fullscale) * gain));
	  }
	} else {
	  --wbridge_ctx[instance].init_counter;
	}

	return ret_val;
}

int32_t BSP_WBRIDGE_DeInit(WheatstoneBridge_t instance) {
  if (instance >= BSP_WBRIDGE_CHANNELS) {
    return BSP_ERROR_WRONG_PARAM;
  }

  int32_t ret_val = BSP_ERROR_NONE;

  if (wbridge_ctx[instance].init_counter > 0U) {
    if (--wbridge_ctx[instance].init_counter == 0U) {
      ret_val = BSP_ADC_DeInit(0U);

      if (ret_val >= BSP_ERROR_NONE) {
        ret_val = BSP_EEPROM_DeInit(0U);
      }
    }
  }

  if (ret_val < BSP_ERROR_NONE) { ++wbridge_ctx[instance].init_counter; }

  return ret_val;
}

int32_t BSP_WBRIDGE_GetVoltage(WheatstoneBridge_t instance, int32_t* p_voltage) {
	if ((instance >= BSP_WBRIDGE_CHANNELS) || (p_voltage == NULL)) {
		return BSP_ERROR_WRONG_PARAM;
	} else if (wbridge_ctx[instance].init_counter == 0U) {
    return BSP_ERROR_NO_INIT;
  } else {
    // Do nothing
  }

  int32_t ret_val = BSP_ADC_GetData(instance, &wbridge_ctx[instance].raw_voltage);

  if (ret_val == BSP_ADC_DATA_READY) {
    // Linear interpolation
    wbridge_ctx[instance].voltage = (((wbridge_ctx[instance].raw_voltage
        * ((wbridge_ctx[instance].cal[1]-wbridge_ctx[instance].cal[0])))
        / wbridge_ctx[instance].raw[1])
        + wbridge_ctx[instance].cal[0]) * 100;
  }

  // Get last successfully read value
  *p_voltage = wbridge_ctx[instance].voltage;

  return ret_val;
}

int32_t BSP_WBRIDGE_GetOutputDataRate(WheatstoneBridge_t instance, float* p_odr) {
	if ((instance >= BSP_WBRIDGE_CHANNELS) || (p_odr == NULL)) {
			return BSP_ERROR_WRONG_PARAM;
  } else if (wbridge_ctx[instance].init_counter == 0U) {
    return BSP_ERROR_NO_INIT;
  } else {
    // Do nothing
  }

	return BSP_ADC_GetOuputDataRate(BSP_WBRIDGE_ADC_START + instance, p_odr);
}

int32_t BSP_WBRIDGE_SetZeroOffset(WheatstoneBridge_t instance, int32_t value) {
  if (instance >= BSP_WBRIDGE_CHANNELS) {
      return BSP_ERROR_WRONG_PARAM;
  } else if (wbridge_ctx[instance].init_counter == 0U) {
    return BSP_ERROR_NO_INIT;
  } else {
    // Do nothing
  }

  uint8_t buffer[2] = { 0U };
  uint32_t address = BSP_EEPROM_WB_MEM_START + (instance * BSP_WBRIDGE_MEM_SIZE);

  wbridge_ctx[instance].zero_offset = ((wbridge_ctx[instance].raw[1] * ((value - wbridge_ctx[instance].cal[0]))
      / (wbridge_ctx[instance].cal[1] - wbridge_ctx[instance].cal[0]))) + 1;

  buffer[0] = wbridge_ctx[instance].zero_offset >> 8U;
  buffer[1] = wbridge_ctx[instance].zero_offset & 0xFF;

  return BSP_EEPROM_WriteBuffer(0U, &buffer[0], address, sizeof(buffer));
}
