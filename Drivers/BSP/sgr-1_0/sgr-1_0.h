/*
 * sgr-1_0.h
 *
 *  Created on: Sep 25, 2024
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_SGR_1_0_SGR_1_0_H_
#define BSP_SGR_1_0_SGR_1_0_H_

#include <stdint.h>

#include "sgr-1_0_conf.h"
#include "sgr-1_0_errno.h"

uint32_t BSP_GetTimeMs(void);
uint32_t BSP_GetAppMemAddress(void);

#endif /* BSP_SGR_1_0_SGR_1_0_H_ */
