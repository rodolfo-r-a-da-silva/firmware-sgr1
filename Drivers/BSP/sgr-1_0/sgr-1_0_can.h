/*
 * sgr-1_0_can.h
 *
 *  Created on: Sep 25, 2024
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_SGR_1_0_SGR_1_0_CAN_H_
#define BSP_SGR_1_0_SGR_1_0_CAN_H_

#include <stdint.h>

#include "can_bus.h"
#include "sgr-1_0_conf.h"
#include "sgr-1_0_errno.h"

#define BUS_CAN1                      CAN1
#define BUS_CAN1_CLK_ENABLE()         __HAL_RCC_CAN1_CLK_ENABLE()
#define BUS_CAN1_CLK_DISABLE()        __HAL_RCC_CAN1_CLK_DISABLE()
#define BUS_CAN1_RX_GPIO_PORT         GPIOA
#define BUS_CAN1_TX_GPIO_PORT         GPIOA
#define BUS_CAN1_RX_GPIO_PIN          GPIO_PIN_11
#define BUS_CAN1_TX_GPIO_PIN          GPIO_PIN_12
#define BUS_CAN1_RX_MODE              GPIO_MODE_INPUT
#define BUS_CAN1_TX_MODE              GPIO_MODE_AF_PP
#define BUS_CAN1_RX_PULL              GPIO_NOPULL
#define BUS_CAN1_TX_SPEED             GPIO_SPEED_FREQ_HIGH
#define BUS_CAN1_RX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_CAN1_TX_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define BUS_CAN1_INT_IRQN             USB_LP_CAN1_RX0_IRQn
#define BUS_CAN1_INT_PREEMPT_PRIORITY	0U
#define BUS_CAN1_INT_SUB_PRIORITY     0U

#ifndef BUS_CAN_FIFO_PRIORITY_HIGH
#define BUS_CAN_FIFO_PRIORITY_HIGH		CAN_RX_FIFO0
#endif

#ifndef BUS_CAN_FIFO_PRIORITY_LOW
#define BUS_CAN_FIFO_PRIORITY_LOW			CAN_RX_FIFO1
#endif

#define BUS_CAN_RX_FILTER_AMOUNT			14U
#define BUS_CAN_RX_FILTER_DISABLED		CAN_FILTER_DISABLE
#define BUS_CAN_RX_FILTER_ENABLED			CAN_FILTER_ENABLE
#define BUS_CAN_RX_FILTER_MODE_MASK		CAN_FILTERMODE_IDMASK
#define BUS_CAN_RX_FILTER_MODE_LIST		CAN_FILTERMODE_IDLIST

#define BUS_CAN_IDE_STD	CAN_ID_STD
#define BUS_CAN_IDE_EXT	CAN_ID_EXT

typedef enum {
	kBusCan1,
	kBusCanAmount
} BusCanTypeDef;

#endif /* BSP_SGR_1_0_SGR_1_0_CAN_H_ */
