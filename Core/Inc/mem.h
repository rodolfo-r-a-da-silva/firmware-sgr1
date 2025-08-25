/*
 * mem.h
 *
 *  Created on: Jul 25, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef LIB_MEM_H_
#define LIB_MEM_H_

#include <stdint.h>

#include "messaging.h"

int32_t mem_load_can_cfg(MessagingCfg_Typedef* p_cfg);
int32_t mem_load_can_msg_freqs(int32_t p_periods[], uint8_t n_of_periods);
int32_t mem_load_filters(uint16_t* p_freq, uint8_t* p_q_fact, uint8_t n_of_filters);

#endif /* LIB_MEM_H_ */
