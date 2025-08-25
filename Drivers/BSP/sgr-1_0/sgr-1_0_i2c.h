/*
 * sgr-1_0_i2c.h
 *
 *  Created on: Mar 29, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_SGR_1_0_SGR_1_0_I2C_H_
#define BSP_SGR_1_0_SGR_1_0_I2C_H_

#include <stdint.h>

#include "sgr-1_0_conf.h"
#include "sgr-1_0_errno.h"

#ifndef BUS_I2C1_FREQUENCY
#define BUS_I2C1_FREQUENCY	100000U
#endif  // BUS_I2C1_FREQUENCY

#define BUS_I2C_TIMEOUT		10U

typedef enum {
	kBusI2c1,
	kBusI2cAmount
} BusI2cTypeDef;

int32_t BSP_I2C1_Init();
int32_t BSP_I2C1_DeInit();
int32_t BSP_I2C1_IsReady(uint16_t addr, uint32_t trials);
int32_t BSP_I2C1_WriteReg(uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length);
int32_t BSP_I2C1_ReadReg(uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length);
int32_t BSP_I2C1_WriteReg16(uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length);
int32_t BSP_I2C1_ReadReg16(uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length);
int32_t BSP_I2C1_Send(uint16_t addr, uint8_t* data, uint16_t length);
int32_t BSP_I2C1_Recv(uint16_t addr, uint8_t* data, uint16_t length);

#endif /* BSP_SGR_1_0_SGR_1_0_I2C_H_ */
