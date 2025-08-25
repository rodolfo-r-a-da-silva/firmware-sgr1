/*
 * temperature.h
 *
 *  Created on: Feb 5, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_COMPONENTS_TEMPERATURE_H_
#define BSP_COMPONENTS_TEMPERATURE_H_

#include <stdint.h>

int32_t BSP_TEMPERATURE_Init();
int32_t BSP_TEMPERATURE_Get(int16_t* p_temperature);

#endif /* BSP_COMPONENTS_TEMPERATURE_H_ */
