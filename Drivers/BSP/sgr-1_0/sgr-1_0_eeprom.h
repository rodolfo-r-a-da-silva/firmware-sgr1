/*
 * sgr-1_0_eeprom.h
 *
 *  Created on: Apr 11, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_SGR_1_0_SGR_1_0_EEPROM_H_
#define BSP_SGR_1_0_SGR_1_0_EEPROM_H_

#include <stdint.h>

#include "Components/M24xx/m24xx.h"
#include "eeprom.h"
#include "sgr-1_0_conf.h"
#include "sgr-1_0_errno.h"

#define BSP_EEPROM_PAGE_SIZE		16U
#define BSP_EEPROM_READY_TRIALS	5U

#endif /* BSP_SGR_1_0_SGR_1_0_EEPROM_H_ */
