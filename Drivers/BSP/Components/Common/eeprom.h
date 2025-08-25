/*
 * eeprom.h
 *
 *  Created on: Apr 8, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_COMPONENTS_COMMON_EEPROM_H_
#define BSP_COMPONENTS_COMMON_EEPROM_H_

#include <stdint.h>

int32_t BSP_EEPROM_Init(uint32_t instance);
int32_t BSP_EEPROM_DeInit(uint32_t instance);
int32_t BSP_EEPROM_WritePage(uint32_t instance, uint8_t* p_data, uint32_t page_nbr);
int32_t BSP_EEPROM_ReadPage(uint32_t instance, uint8_t* p_data, uint32_t page_nbr);
int32_t BSP_EEPROM_WriteBuffer(uint32_t instance, uint8_t* p_data, uint32_t write_addr, uint32_t length);
int32_t BSP_EEPROM_ReadBuffer(uint32_t instance, uint8_t* p_data, uint32_t read_addr, uint32_t length);
int32_t BSP_EEPROM_IsReady(uint32_t instance);

#endif /* BSP_COMPONENTS_COMMON_EEPROM_H_ */
