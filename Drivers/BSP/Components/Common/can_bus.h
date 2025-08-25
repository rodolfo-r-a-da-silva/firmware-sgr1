/*
 * can_bus.h
 *
 *  Created on: Jun 12, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef BSP_COMPONENTS_COMMON_CAN_BUS_H_
#define BSP_COMPONENTS_COMMON_CAN_BUS_H_

#include <stdint.h>

typedef enum {
  kBspCanBaudRate10kbps = 0,
  kBspCanBaudRate20kbps,
  kBspCanBaudRate50kbps,
  kBspCanBaudRate125kbps,
  kBspCanBaudRate250kbps,
  kBspCanBaudRate500kbps,
  kBspCanBaudRate800kbps,
  kBspCanBaudRate1000kbps
} CanBus_BaudRate_TypeDef;

typedef struct {
  CanBus_BaudRate_TypeDef baud_rate;
} CanBus_Config_TypeDef;

typedef struct {
  uint32_t id;
  uint32_t mask;

  uint8_t enabled;
  uint8_t fifo_priority;
  uint8_t mode;

  uint8_t instance;
} CanBus_RecvFilter_TypeDef;

typedef struct {
  union {
    uint8_t data8[8];
    uint16_t data16[4];
    uint32_t data32[2];
  };

  uint8_t size;
  uint8_t ide;
  uint32_t id;
} CanBus_FrameData_TypeDef;

int32_t BSP_CAN_BUS_Init(uint32_t instance, const CanBus_Config_TypeDef* config);
int32_t BSP_CAN_BUS_DeInit(uint32_t instance);
int32_t BSP_CAN_BUS_SetFilters(uint32_t instance, const CanBus_RecvFilter_TypeDef* filters, uint8_t filters_amount);
int32_t BSP_CAN_BUS_Start(uint32_t instance);
int32_t BSP_CAN_BUS_Stop(uint32_t instance);
int32_t BSP_CAN_BUS_Recv(uint32_t instance, CanBus_FrameData_TypeDef* frame);
int32_t BSP_CAN_BUS_Send(uint32_t instance, const CanBus_FrameData_TypeDef* frame);

#endif /* BSP_COMPONENTS_COMMON_CAN_BUS_H_ */
