/*
 * messaging.c
 *
 *  Created on: Jan 26, 2025
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#include <assert.h>
#include <stddef.h>

#include "messaging.h"

static uint32_t first_frame_id = 0x0;

/**
 * @brief  Initializes and starts CAN bus communication
 * @param  first_id the ID of the data packet containing the first strain gauges
 * @retval BSP Error code
 */
int32_t can_messaging_init(MessagingCfg_Typedef *p_cfg) {
  assert(p_cfg != NULL);

	int32_t ret_val = 0;
	first_frame_id = p_cfg->first_id;
	CanBus_Config_TypeDef can_config = { .baud_rate = p_cfg->baud_rate };

	ret_val = BSP_CAN_BUS_Init(0U, &can_config);

	if (ret_val >= 0) {
		ret_val = BSP_CAN_BUS_Start(0U);
	}

	return ret_val;
}

int32_t can_messaging_send(const uint8_t data_packet, const uint32_t* p_data, const uint8_t* p_verify) {
	assert(data_packet < CAN_MESSAGING_TOTAL_MESSAGES);
	assert(p_data != NULL);
	assert(p_verify != NULL);

	CanBus_FrameData_TypeDef frame = { .id = first_frame_id + data_packet,
															       .ide = BUS_CAN_IDE_EXT,
															       .size = 0U,
															       .data8 = { 0U } };

	if (data_packet < CAN_MESSAGING_DATA_MESSAGES) {
		for (uint8_t i = data_packet * CAN_MESSAGING_FRAME_CHANNELS;
				 (i < BSP_WBRIDGE_CHANNELS) && (frame.size < CAN_MESSAGING_FRAME_BYTES); ++i, frame.size += 2U) {
			frame.data16[i] = (uint16_t) (p_data[i] & 0xffff);
		}
	} else if (data_packet == CAN_MESSAGING_DIAG_MESSAGE) {
		frame.size = CAN_MESSAGING_DIAG_MESSAGE_SIZE;
		assert(BSP_TEMPERATURE_Get((int16_t*) &frame.data16[0]) >= BSP_ERROR_NONE);

		for (uint8_t i = 0U, j = 1U; i < BSP_WBRIDGE_CHANNELS; ++i) {
			if ((i % 8U) == 0U) {
				++j;
			}

			frame.data8[j] |= p_verify[i] << (i % 8U);
		}
		// TODO: add fixed messages frequency transmission?
	} else {
		return BSP_ERROR_WRONG_PARAM;
	}

	return BSP_CAN_BUS_Send(0U, &frame);
}

int32_t can_messaging_freq_to_ms(MessagingFreqs_TypeDef freq) {
  switch (freq) {
  case kMessage1Hz:
    return 1000;
  case kMessage2Hz:
    return 500;
  case kMessage5Hz:
    return 200;
  case kMessage10Hz:
    return 100;
  case kMessage20Hz:
    return 50;
  case kMessage50Hz:
    return 20;
  case kMessage100Hz:
    return 10;
  case kMessage200Hz:
    return 5;
  case kMessage500Hz:
    return 2;
  case kMessage1000Hz:
    return 1;
  default:
    return CAN_MESSAGING_MESSAGE_DISABLED;
  }
}
