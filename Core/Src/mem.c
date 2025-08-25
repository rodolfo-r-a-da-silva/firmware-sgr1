/*
 * mem.c
 *
 *  Created on: Jul 25, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef LIB_MEM_C_
#define LIB_MEM_C_

#include <stddef.h>

#include "mem.h"

#include "bsp.h"
#include "eeprom.h"

#define MEM_CAN_CFG_SIZE 9U
#define MEM_CAN_MSG_FREQ_SIZE 4U
#define MEM_FILTER_SIZE 3U

int32_t mem_load_can_cfg(MessagingCfg_Typedef* p_cfg) {
  if (p_cfg == NULL) { return -1; }

  uint8_t buffer[MEM_CAN_CFG_SIZE];
  uint32_t addr = BSP_GetAppMemAddress();

  int32_t ret = BSP_EEPROM_ReadBuffer(0U, &buffer[0], addr, sizeof(buffer));

  if (ret >= 0) {
    p_cfg->baud_rate = buffer[0];

    p_cfg->filter_id  = buffer[1] << 24U;
    p_cfg->filter_id |= buffer[2] << 16U;
    p_cfg->filter_id |= buffer[3] << 8U;
    p_cfg->filter_id |= buffer[4];

    p_cfg->first_id  = buffer[5] << 24U;
    p_cfg->first_id |= buffer[6] << 16U;
    p_cfg->first_id |= buffer[7] << 8U;
    p_cfg->first_id |= buffer[8];
  }

  return ret;
}

int32_t mem_load_can_msg_freqs(int32_t p_periods[], uint8_t n_of_periods) {
  if (p_periods == NULL) { return -1; }

  uint8_t buffer[MEM_CAN_MSG_FREQ_SIZE];
  uint32_t addr = BSP_GetAppMemAddress() + MEM_CAN_CFG_SIZE;

  int32_t ret = BSP_EEPROM_ReadBuffer(0U, &buffer[0], addr, sizeof(buffer));

  for (uint8_t i = 0U; i < n_of_periods; i++) {
    p_periods[i] = can_messaging_freq_to_ms(buffer[i]);
  }

  return ret;
}

int32_t mem_load_filters(uint16_t p_freq[], uint8_t p_q_fact[], uint8_t n_of_filters) {
  if ((p_freq == NULL) && (p_q_fact == NULL)) {
    return -1;
  }

  uint8_t buffer[MEM_FILTER_SIZE];
  uint32_t addr = BSP_GetAppMemAddress() + MEM_CAN_CFG_SIZE + MEM_CAN_MSG_FREQ_SIZE;

  for (uint8_t i = 0U; i < n_of_filters; ++i, addr += MEM_FILTER_SIZE) {
    if (BSP_EEPROM_ReadBuffer(0U, &buffer[0], addr, MEM_FILTER_SIZE) == 0) {
      p_freq[i] = (buffer[0] << 8U) | buffer[1];
      p_q_fact[i] = buffer[2];
    } else {
      return -1;
    }
  }
  return 0;
}

#endif /* LIB_MEM_C_ */
