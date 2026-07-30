//
// Created by Dolores on 2026/7/30.
//

#ifndef GIMBALL_BSP_CAN_H
#define GIMBALL_BSP_CAN_H

#include "main.h"
#include "can.h"

#define BSP_CAN_RX_FIFO_SIZE 8U

typedef struct {
    CAN_RxHeaderTypeDef header;
    uint8_t data[8];
} BSP_CAN_RxMsg_t;

void BSP_CAN_FilterInit(void);
HAL_StatusTypeDef BSP_CAN_SendMessage(uint32_t std_id, uint8_t data[8], uint32_t timeout);
uint8_t BSP_CAN_GetRxMessage(BSP_CAN_RxMsg_t *msg);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan);
uint32_t BSP_CAN_GetTxErrorCount(void);
uint32_t BSP_CAN_GetRxErrorCount(void);
void BSP_CAN_ClearErrorCounters(void);

#endif //GIMBALL_BSP_CAN_H
