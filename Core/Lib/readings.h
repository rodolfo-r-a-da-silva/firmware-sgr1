/*
 * readings.h
 *
 *  Created on: Nov 5, 2024
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef INC_READINGS_H_
#define INC_READINGS_H_

#include <stdint.h>

#include "bsp.h"
#include "Lib/dsp.h"
#include "wheatstone_bridge.h"

typedef struct {
  WheatstoneBridge_t wbridge;
  int32_t input;
	int32_t* p_output;
	uint8_t* p_verify;
	int32_t err;

	uint8_t filter_init;
	DspFilterTypeDef filter;
} ReadingTypedef;

int32_t readings_init(ReadingTypedef* p_readings, WheatstoneBridge_t wbridge_nbr, uint32_t* p_output,
                      uint8_t* p_verify);
int32_t readings_set_filter(ReadingTypedef* p_readings, uint16_t* p_freq, uint8_t* p_q_factor);
int32_t readings_calc(ReadingTypedef* p_readings);

#endif /* INC_READINGS_H_ */
