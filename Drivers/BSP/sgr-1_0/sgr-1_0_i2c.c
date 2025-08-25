/*
 * sgr-1_0_i2c.c
 *
 *  Created on: Mar 29, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#include "sgr-1_0_i2c.h"

static int32_t I2C_Init(BusI2cTypeDef instance);
static int32_t I2C_DeInit(BusI2cTypeDef instance);
static int32_t I2C_IsReady(BusI2cTypeDef instance, uint16_t addr, uint32_t trials);
static int32_t I2C_WriteReg(BusI2cTypeDef instance, uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length);
static int32_t I2C_ReadReg(BusI2cTypeDef instance, uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length);
static int32_t I2C_WriteReg16(BusI2cTypeDef instance, uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length);
static int32_t I2C_ReadReg16(BusI2cTypeDef instance, uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length);
static int32_t I2C_Send(BusI2cTypeDef instance, uint16_t addr, uint8_t* data, uint16_t length);
static int32_t I2C_Recv(BusI2cTypeDef instance, uint16_t addr, uint8_t* data, uint16_t length);
static void SetInstances();
static int32_t ReturnError(HAL_StatusTypeDef status);

extern I2C_HandleTypeDef hi2c1;
// Access i2c bus peripherals
static I2C_HandleTypeDef* hbus_i2c[kBusI2cAmount];
// Make sure peripheral is not initialized multiple times or de-initialized when in use
static uint8_t init_counter_i2c[kBusI2cAmount] = { 0U };

int32_t BSP_I2C1_Init() {
	return I2C_Init(kBusI2c1);
}

int32_t BSP_I2C1_DeInit() {
	return I2C_DeInit(kBusI2c1);
}

int32_t BSP_I2C1_IsReady(uint16_t addr, uint32_t trials) {
	return I2C_IsReady(kBusI2c1, addr, trials);
}

int32_t BSP_I2C1_WriteReg(uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length) {
	return I2C_WriteReg(kBusI2c1, addr, reg, data, length);
}

int32_t BSP_I2C1_ReadReg(uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length) {
	return I2C_ReadReg(kBusI2c1, addr, reg, data, length);
}

int32_t BSP_I2C1_WriteReg16(uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length) {
	return I2C_WriteReg16(kBusI2c1, addr, reg, data, length);
}

int32_t BSP_I2C1_ReadReg16(uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length) {
	return I2C_ReadReg16(kBusI2c1, addr, reg, data, length);
}

int32_t BSP_I2C1_Send(uint16_t addr, uint8_t* data, uint16_t length) {
	return I2C_Send(kBusI2c1, addr, data, length);
}

int32_t BSP_I2C1_Recv(uint16_t addr, uint8_t* data, uint16_t length) {
	return I2C_Recv(kBusI2c1, addr, data, length);
}


static int32_t I2C_Init(BusI2cTypeDef instance) {
	if ((instance < kBusI2c1) || (instance >= kBusI2cAmount)) {
		return BSP_ERROR_WRONG_PARAM;
	}

	int32_t ret = BSP_ERROR_NONE;

	SetInstances();

	if ((init_counter_i2c[instance]++ == 0U) && (HAL_I2C_GetState(hbus_i2c[instance]) == HAL_I2C_STATE_RESET)) {
		if (ret != BSP_ERROR_NONE) {
			--init_counter_i2c[instance];
		}
	}

	return ret;
}

static int32_t I2C_DeInit(BusI2cTypeDef instance) {
	if ((instance < kBusI2c1) || (instance >= kBusI2cAmount)) {
		return BSP_ERROR_WRONG_PARAM;
	}

	int32_t ret = BSP_ERROR_NONE;

	if (init_counter_i2c[instance] > 0U) {
		if (--init_counter_i2c[instance] == 0U){
			if (HAL_I2C_DeInit(hbus_i2c[instance]) != HAL_OK) {
				ret = BSP_ERROR_BUS_FAILURE;
			}
		}
	}

	return ret;
}

static int32_t I2C_IsReady(BusI2cTypeDef instance, uint16_t addr, uint32_t trials) {
	if ((instance < kBusI2c1) || (instance >= kBusI2cAmount)) {
		return BSP_ERROR_WRONG_PARAM;
	}

	HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hbus_i2c[instance], addr, trials, BUS_I2C_TIMEOUT);

	return ReturnError(status);
}

static int32_t I2C_WriteReg(BusI2cTypeDef instance, uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length) {
	if ((instance < kBusI2c1) || (instance >= kBusI2cAmount)) {
		return BSP_ERROR_WRONG_PARAM;
	}

	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hbus_i2c[instance], addr, reg, 1U, data, length, BUS_I2C_TIMEOUT);

	return ReturnError(status);
}

static int32_t I2C_ReadReg(BusI2cTypeDef instance, uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length) {
	if ((instance < kBusI2c1) || (instance >= kBusI2cAmount)) {
		return BSP_ERROR_WRONG_PARAM;
	}

	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hbus_i2c[instance], addr, reg, 1U, data, length, BUS_I2C_TIMEOUT);

	return ReturnError(status);
}

static int32_t I2C_WriteReg16(BusI2cTypeDef instance, uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length) {
	if ((instance < kBusI2c1) || (instance >= kBusI2cAmount)) {
		return BSP_ERROR_WRONG_PARAM;
	}

	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hbus_i2c[instance], addr, reg, 2U, data, length, BUS_I2C_TIMEOUT);

	return ReturnError(status);
}

static int32_t I2C_ReadReg16(BusI2cTypeDef instance, uint16_t addr, uint16_t reg, uint8_t* data, uint16_t length) {
	if ((instance < kBusI2c1) || (instance >= kBusI2cAmount)) {
		return BSP_ERROR_WRONG_PARAM;
	}

	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hbus_i2c[instance], addr, reg, 2U, data, length, BUS_I2C_TIMEOUT);

	return ReturnError(status);
}

static int32_t I2C_Send(BusI2cTypeDef instance, uint16_t addr, uint8_t* data, uint16_t length) {
	if ((instance < kBusI2c1) || (instance >= kBusI2cAmount)) {
		return BSP_ERROR_WRONG_PARAM;
	}

	HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hbus_i2c[instance], addr, data,  length, BUS_I2C_TIMEOUT);

	return ReturnError(status);
}

static int32_t I2C_Recv(BusI2cTypeDef instance, uint16_t addr, uint8_t* data, uint16_t length) {
	if ((instance < kBusI2c1) || (instance >= kBusI2cAmount)) {
		return BSP_ERROR_WRONG_PARAM;
	}

	HAL_StatusTypeDef status = HAL_I2C_Master_Receive(hbus_i2c[instance], addr | 1U, data, length, BUS_I2C_TIMEOUT);

	return ReturnError(status);
}

static void SetInstances() {
	hbus_i2c[kBusI2c1] = &hi2c1;
}

static int32_t ReturnError(HAL_StatusTypeDef status) {
	switch (status) {
	case HAL_OK:
		return BSP_ERROR_NONE;
	case HAL_ERROR:
		return BSP_ERROR_PERIPH_FAILURE;
	case HAL_BUSY:
		return BSP_ERROR_BUSY;
	case HAL_TIMEOUT:
		return BSP_ERROR_COMPONENT_FAILURE;
	default:
		break;
	}

	return BSP_ERROR_UNKNOWN_FAILURE;
}
