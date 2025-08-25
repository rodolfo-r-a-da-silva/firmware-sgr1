/*
 * wheatstone_bridge.h
 *
 *  Created on: Feb 7, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_COMPONENTS_COMMON_WHEATSTONE_BRIDGE_H_
#define BSP_COMPONENTS_COMMON_WHEATSTONE_BRIDGE_H_

#include <stdint.h>

#define BSP_WBRIDGE_STRAIN_NOT_READY	0
#define BSP_WBRIDGE_STRAIN_READY			1

typedef uint8_t WheatstoneBridge_t;

int32_t BSP_WBRIDGE_Init(WheatstoneBridge_t instance);
int32_t BSP_WBRIDGE_DeInit(WheatstoneBridge_t instance);
int32_t BSP_WBRIDGE_GetVoltage(WheatstoneBridge_t instance, int32_t* p_voltage);
int32_t BSP_WBRIDGE_GetOutputDataRate(WheatstoneBridge_t instance, float* p_odr);
int32_t BSP_WBRIDGE_SetZeroOffset(WheatstoneBridge_t instance, int32_t value);

#endif /* BSP_COMPONENTS_COMMON_WHEATSTONE_BRIDGE_H_ */
