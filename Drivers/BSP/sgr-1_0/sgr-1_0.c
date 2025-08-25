/*
 * sgr-1_0.c
 *
 *  Created on: May 3, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#include "sgr-1_0.h"

inline uint32_t BSP_GetTimeMs(void) {
  return HAL_GetTick();
}

inline uint32_t BSP_GetAppMemAddress(void) {
  return BSP_EEPROM_APP_MEM_START;
}
