/*
 * sgr-1_0_eeprom.c
 *
 *  Created on: Apr 11, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#include <string.h>

#include "sgr-1_0_eeprom.h"

static const uint16_t page_size = BSP_EEPROM_PAGE_SIZE;
static M24_Object_t eeprom;

static const uint8_t mem[256] = {
    // Wheatstone bridge data
    0x3F, 0xFF, 0x0C, 0xEE,
    0x3F, 0xFF, 0x0C, 0xEE,
    0x3F, 0xFF, 0x0C, 0xEE,
    0x3F, 0xFF, 0x0C, 0xEE,
    0x3F, 0xFF, 0x0C, 0xEE,
    0x3F, 0xFF, 0x0C, 0xEE,
    0x3F, 0xFF, 0x0C, 0xEE,
    0x3F, 0xFF, 0x0C, 0xEE,
    0x3F, 0xFF, 0x0C, 0xEE,
    0x3F, 0xFF, 0x0C, 0xEE,
    // CAN baud rate
    0x00,
    // CAN filter ID
    0x1F, 0xFF, 0xFF, 0xFF,
    // CAN first ID
    0x1F, 0xFF, 0xFF, 0xFF,
    // CAN message periods
    0x08, 0x08, 0x08, 0x08,
    // Filter data
    0x00, 0x0A, 0x3C,
    0x00, 0x0A, 0x3C,
    0x00, 0x0A, 0x3C,
    0x00, 0x0A, 0x3C,
    0x00, 0x0A, 0x3C,
    0x00, 0x0A, 0x3C,
    0x00, 0x0A, 0x3C,
    0x00, 0x0A, 0x3C,
    0x00, 0x0A, 0x3C,
    0x00, 0x0A, 0x3C
};

int32_t BSP_EEPROM_Init(uint32_t instance) {
  return BSP_ERROR_NONE;
  UNUSED(instance);
  M24_IO_t eeprom_io = { .Init = &BSP_I2C1_Init,
                         .DeInit = &BSP_I2C1_DeInit,
                         .WriteReg = &BSP_I2C1_WriteReg,
                         .WriteReg16 = &BSP_I2C1_WriteReg16,
                         .Transmit = &BSP_I2C1_Send,
                         .ReadReg = &BSP_I2C1_ReadReg,
                         .ReadReg16 = &BSP_I2C1_ReadReg16,
                         .IsReady = &BSP_I2C1_IsReady,
                         .Delay = &HAL_Delay,
                         .Address = 0xA0 };

  if (M24_RegisterBusIO(&eeprom, &eeprom_io) != M24_OK) {
    return BSP_ERROR_UNKNOWN_FAILURE;
  }

  if (M24_i2c_Init(&eeprom) != M24_OK) {
    return BSP_ERROR_COMPONENT_FAILURE;
  }

  return BSP_ERROR_NONE;
}

int32_t BSP_EEPROM_DeInit(uint32_t instance) {
  UNUSED(instance);
  if (M24_i2c_DeInit(&eeprom) != M24_OK) {
    return BSP_ERROR_COMPONENT_FAILURE;
  }
  return BSP_ERROR_NONE;
}

int32_t BSP_EEPROM_WritePage(uint32_t instance, uint8_t* p_data, uint32_t page_nbr) {
  UNUSED(instance);
  int32_t ret_val = M24_i2c_WritePage(&eeprom, p_data, page_nbr * page_size, page_size, page_size);

  if (ret_val == M24_BUSY) {
    return BSP_ERROR_BUSY;
  }

  return ret_val;
}

int32_t BSP_EEPROM_ReadPage(uint32_t instance, uint8_t* p_data, uint32_t page_nbr) {
  UNUSED(instance);
  int32_t ret_val = M24_i2c_ReadPage(&eeprom, p_data, page_nbr * page_size, page_size);

  if (ret_val == M24_BUSY) {
    return BSP_ERROR_BUSY;
  }

  return ret_val;
}

int32_t BSP_EEPROM_WriteBuffer(uint32_t instance, uint8_t* p_data, uint32_t write_addr, uint32_t length) {
  UNUSED(instance);
  int32_t ret_val = M24_i2c_WriteData(&eeprom, p_data, write_addr, page_size, length);

  if (ret_val == M24_BUSY) {
    return BSP_ERROR_BUSY;
  }

  return ret_val;
}

int32_t BSP_EEPROM_ReadBuffer(uint32_t instance, uint8_t* p_data, uint32_t read_addr, uint32_t length) {
  memcpy(&p_data[0], &mem[read_addr], length);
  return BSP_ERROR_NONE;
  UNUSED(instance);
  int32_t ret_val = M24_i2c_ReadData(&eeprom, p_data, read_addr, length);

  if (ret_val == M24_BUSY) {
    return BSP_ERROR_BUSY;
  }

  return ret_val;
}

int32_t BSP_EEPROM_IsReady(uint32_t instance) {
  UNUSED(instance);
  return M24_i2c_IsDeviceReady(&eeprom, BSP_EEPROM_READY_TRIALS);
}
