/*
 * messaging.h
 *
 *  Created on: Jan 26, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#ifndef LIB_MESSAGING_H_
#define LIB_MESSAGING_H_

#include <stdint.h>

#include "bsp.h"
#include "can_bus.h"

#define CAN_MESSAGING_FRAME_BYTES				8U
#define CAN_MESSAGING_FRAME_CHANNELS		(CAN_MESSAGING_FRAME_BYTES / 2U)
#define CAN_MESSAGING_DATA_MESSAGES			((BSP_WBRIDGE_CHANNELS + CAN_MESSAGING_FRAME_CHANNELS - 1U) \
																					/ CAN_MESSAGING_FRAME_CHANNELS)
#define CAN_MESSAGING_DIAG_MESSAGE			CAN_MESSAGING_DATA_MESSAGES
#define CAN_MESSAGING_DIAG_MESSAGE_SIZE	(2U + ((BSP_WBRIDGE_CHANNELS + 7U) / 8U))
#define CAN_MESSAGING_TOTAL_MESSAGES		(CAN_MESSAGING_DATA_MESSAGES + 1U)

#define CAN_MESSAGING_MESSAGE_DISABLED -1

typedef enum {
  kMessageDisabled = 0U,
  kMessage1Hz,
  kMessage2Hz,
  kMessage5Hz,
  kMessage10Hz,
  kMessage20Hz,
  kMessage50Hz,
  kMessage100Hz,
  kMessage200Hz,
  kMessage500Hz,
  kMessage1000Hz
} MessagingFreqs_TypeDef;

typedef struct {
  CanBus_BaudRate_TypeDef baud_rate;

  uint32_t first_id;
  uint32_t filter_id;
  uint32_t filter_mask;

  uint32_t msg_period[CAN_MESSAGING_TOTAL_MESSAGES];
} MessagingCfg_Typedef;

int32_t can_messaging_init(MessagingCfg_Typedef *p_cfg);
int32_t can_messaging_send(const uint8_t data_packet, const uint32_t* p_data, const uint8_t* p_verify);
int32_t can_messaging_freq_to_ms(MessagingFreqs_TypeDef freq);

#endif /* LIB_MESSAGING_H_ */
