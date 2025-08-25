/*
 * sgr-1_0_can.c
 *
 *  Created on: Sep 25, 2024
 *      Author: Rodolfo Rodrigues Alcantara da Silva
 */

#include "sgr-1_0_can.h"

static void SetInstances();
static int32_t GetPrescalerAndTimeSegs(CAN_InitTypeDef* can_init, CanBus_BaudRate_TypeDef baud_rate);

extern CAN_HandleTypeDef hcan;
// Access can bus peripherals
static CAN_HandleTypeDef* hbus_can[kBusCanAmount];
// Make sure peripheral is not initialized multiple times or de-initialized when in use
static uint8_t init_counter_can[kBusCanAmount] = { 0U };

int32_t BSP_CAN_BUS_Init(uint32_t instance, const CanBus_Config_TypeDef* config) {
	if (instance >= kBusCanAmount) {
		return BSP_ERROR_WRONG_PARAM;
	}

	int32_t ret = BSP_ERROR_NONE;

	SetInstances();

	if ((init_counter_can[instance]++ == 0U) && (HAL_CAN_GetState(hbus_can[instance]) == HAL_CAN_STATE_RESET)) {
		ret = GetPrescalerAndTimeSegs(&hbus_can[instance]->Init, config->baud_rate);

//		if (ret == BSP_ERROR_NONE) {
//			if (HAL_CAN_Init(hbus_can[instance]) != HAL_OK) {
//				ret = BSP_ERROR_BUS_FAILURE;
//			}
//		}

		if (ret != BSP_ERROR_NONE) {
			--init_counter_can[instance];
		}
	}

	return ret;
}

int32_t BSP_CAN_BUS_DeInit(uint32_t instance) {
	if (instance >= kBusCanAmount) {
		return BSP_ERROR_WRONG_PARAM;
	}

	int32_t ret = BSP_ERROR_NONE;

	if (init_counter_can[instance] > 0U) {
		if (--init_counter_can[instance] == 0U){
			if (HAL_CAN_DeInit(hbus_can[instance]) != HAL_OK) {
				ret = BSP_ERROR_BUS_FAILURE;
			}
		}
	}

	return ret;
}

int32_t BSP_CAN_BUS_SetFilters(uint32_t instance, const CanBus_RecvFilter_TypeDef* filters, uint8_t filters_amount) {
	if ((instance >= kBusCanAmount) || (filters_amount > BUS_CAN_RX_FILTER_AMOUNT)) {
		return BSP_ERROR_WRONG_PARAM;
	} else if (init_counter_can[instance] == 0U) {
		return BSP_ERROR_NO_INIT;
	}

	int32_t ret = BSP_ERROR_NONE;
	CAN_FilterTypeDef can_config_filter = { .FilterScale = CAN_FILTERSCALE_32BIT,
																					.FilterActivation = CAN_FILTER_ENABLE,
																					.SlaveStartFilterBank = 0U };

	for (uint8_t i = 0; i < filters_amount; ++i) {
		uint32_t j = 0;
		if (filters[i].enabled == BUS_CAN_RX_FILTER_ENABLED) {
			can_config_filter.FilterIdHigh = (uint32_t) ((filters[j].id >> 16) & 0xffff);
			can_config_filter.FilterIdLow = (uint32_t) (filters[j].id & 0xffff);
			can_config_filter.FilterMaskIdHigh = (uint32_t) ((filters[j].mask >> 16) & 0xffff);
			can_config_filter.FilterMaskIdLow = (uint32_t) (filters[j].mask & 0xffff);
			can_config_filter.FilterFIFOAssignment = (uint32_t) filters[j].fifo_priority;
			can_config_filter.FilterBank = j;
			can_config_filter.FilterMode = (uint32_t) filters[j].mode;

			++j;

			if (HAL_CAN_ConfigFilter(hbus_can[instance], &can_config_filter) != HAL_OK) {
				ret = BSP_ERROR_NO_INIT;
				break;
			}
		}
	}

	return ret;
}

int32_t BSP_CAN_BUS_Start(uint32_t instance) {
	if (instance >= kBusCanAmount) {
		return BSP_ERROR_WRONG_PARAM;
	} else if (init_counter_can[instance] ==  0U) {
		return BSP_ERROR_NO_INIT;
	}

	int32_t ret = BSP_ERROR_NONE;

	if (HAL_CAN_Start(hbus_can[instance]) != HAL_OK) {
		ret = BSP_ERROR_BUS_FAILURE;
	}

	return ret;
}

int32_t BSP_CAN_BUS_Stop(uint32_t instance) {
	if (instance >= kBusCanAmount) {
			return BSP_ERROR_WRONG_PARAM;
	} else if (init_counter_can[instance] == 0U) {
		return BSP_ERROR_NO_INIT;
	}

	int32_t ret = BSP_ERROR_NONE;

	if (HAL_CAN_Stop(hbus_can[instance]) != HAL_OK) {
		ret = BSP_ERROR_BUS_FAILURE;
	}

	return ret;
}

int32_t BSP_CAN_BUS_Recv(uint32_t instance, CanBus_FrameData_TypeDef* frame) {
	if (frame == NULL) {
		return BSP_ERROR_WRONG_PARAM;
	} else if (instance >= kBusCanAmount) {
		return BSP_ERROR_WRONG_PARAM;
	}else if (init_counter_can[instance] == 0U) {
		return BSP_ERROR_NO_INIT;
	}

	int32_t ret = 0;
	uint8_t buffer[8] = { 0 };
	uint32_t fifo = BUS_CAN_FIFO_PRIORITY_LOW;
	CAN_RxHeaderTypeDef rx_header = { 0 };

	ret += (int32_t) HAL_CAN_GetRxFifoFillLevel(hbus_can[instance], BUS_CAN_FIFO_PRIORITY_HIGH);

	if (ret > 0) {
		fifo = BUS_CAN_FIFO_PRIORITY_HIGH;
	}

	ret += (int32_t) HAL_CAN_GetRxFifoFillLevel(hbus_can[instance], BUS_CAN_FIFO_PRIORITY_LOW);

	if (ret == 0) {
		return 0;
	}

	if (HAL_CAN_GetRxMessage(hbus_can[instance], fifo, &rx_header, &buffer[0]) == HAL_OK) {
		frame->id = (rx_header.IDE = BUS_CAN_IDE_STD) ? rx_header.StdId : rx_header.ExtId;
		frame->ide = (uint8_t) rx_header.IDE;
		frame->size = (uint8_t) rx_header.DLC;
		for (uint32_t i = 0; i < rx_header.DLC; ++i) {
			frame->data8[i] = buffer[i];
		}

		--ret;
	} else {
		return BSP_ERROR_WRONG_PARAM;
	}

	return ret;
}

int32_t BSP_CAN_BUS_Send(uint32_t instance, const CanBus_FrameData_TypeDef* frame) {
	if (frame == NULL) {
		return BSP_ERROR_WRONG_PARAM;
	} else if (instance >= kBusCanAmount) {
		return BSP_ERROR_WRONG_PARAM;
	} else if (init_counter_can[instance] == 0U) {
		return BSP_ERROR_NO_INIT;
	}

	int32_t ret = BSP_ERROR_NONE;

	uint32_t tx_mailbox;
	CAN_TxHeaderTypeDef tx_header = { 0 };

	tx_header.DLC = frame->size;

	if (frame->ide == 0U) {
		tx_header.IDE = BUS_CAN_IDE_STD;
		tx_header.StdId = frame->id;
	} else {
		tx_header.IDE = BUS_CAN_IDE_EXT;
		tx_header.ExtId = frame->id;
	}

	if (HAL_CAN_AddTxMessage(hbus_can[instance], &tx_header, &frame->data8[0], &tx_mailbox) != HAL_OK) {
		ret = BSP_ERROR_WRONG_PARAM;
	}

	return ret;
}

//void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan) {
//	GPIO_InitTypeDef gpio_init = {0};
//
//  if (hcan->Instance == BUS_CAN1) {
//		// Enable GPIO  clock
//		BUS_CAN1_RX_GPIO_CLK_ENABLE();
//		BUS_CAN1_TX_GPIO_CLK_ENABLE();
//
//		// Enable main peripheral clock
//		BUS_CAN1_CLK_ENABLE();
//
//		// Configure CAN RX as input
//		gpio_init.Pin = BUS_CAN1_RX_GPIO_PIN;
//		gpio_init.Mode = BUS_CAN1_RX_MODE;
//		gpio_init.Pull = BUS_CAN1_RX_PULL;
//		HAL_GPIO_Init(BUS_CAN1_RX_GPIO_PORT, &gpio_init);
//
//		// Configure CAN TX as alternate function
//		gpio_init.Pin = BUS_CAN1_TX_GPIO_PIN;
//		gpio_init.Mode = BUS_CAN1_TX_MODE;
//		gpio_init.Speed = BUS_CAN1_TX_SPEED;
//		HAL_GPIO_Init(BUS_CAN1_TX_GPIO_PORT, &gpio_init);
//
//		// Configure CAN interrupt
//		HAL_NVIC_SetPriority(BUS_CAN1_INT_IRQN, BUS_CAN1_INT_PREEMPT_PRIORITY, BUS_CAN1_INT_SUB_PRIORITY);
//		HAL_NVIC_EnableIRQ(BUS_CAN1_INT_IRQN);
//  }
//}

static void SetInstances() {
	hbus_can[kBusCan1] = &hcan;
}

static int32_t GetPrescalerAndTimeSegs(CAN_InitTypeDef* can_init, CanBus_BaudRate_TypeDef baud_rate) {
  if (can_init == NULL) { return BSP_ERROR_WRONG_PARAM; }

	int32_t ret = BSP_ERROR_NONE;

	can_init->TimeSeg1 = CAN_BS1_7TQ;
	can_init->TimeSeg2 = CAN_BS2_1TQ;

	switch (baud_rate) {
	case kBspCanBaudRate10kbps:
		can_init->Prescaler = 400;
		break;
	case kBspCanBaudRate20kbps:
		can_init->Prescaler = 200;
		break;
	case kBspCanBaudRate50kbps:
		can_init->Prescaler = 80;
		break;
	case kBspCanBaudRate125kbps:
		can_init->Prescaler = 32;
		break;
	case kBspCanBaudRate250kbps:
		can_init->Prescaler = 16;
		break;
	case kBspCanBaudRate500kbps:
		can_init->Prescaler = 8;
		break;
	case kBspCanBaudRate800kbps:
		can_init->Prescaler = 5;
		break;
	case kBspCanBaudRate1000kbps:
		can_init->Prescaler = 4;
		break;
	default:
		ret = BSP_ERROR_WRONG_PARAM;
		break;
	}

	return ret;
}

__weak HAL_StatusTypeDef MxCanInit(CAN_HandleTypeDef* hcan, CanBus_Config_TypeDef* can_config) {
	// Bit timing configuration
	hcan->Init.Prescaler = 4;
	hcan->Init.TimeSeg1 = CAN_BS1_7TQ;
	hcan->Init.TimeSeg2 = CAN_BS2_1TQ;
	hcan->Init.SyncJumpWidth = CAN_SJW_1TQ;

	hcan->Init.Mode = CAN_MODE_NORMAL;
	hcan->Init.AutoBusOff = DISABLE;
	hcan->Init.AutoWakeUp = DISABLE;
	hcan->Init.AutoRetransmission = DISABLE;
	hcan->Init.ReceiveFifoLocked = DISABLE;
	hcan->Init.TransmitFifoPriority = DISABLE;

	// Not supported by hardware
	hcan->Init.TimeTriggeredMode = DISABLE;
	return HAL_CAN_Init(hcan);
}
