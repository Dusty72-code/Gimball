//
// Created by Dolores on 2026/7/30.
//

#ifndef GIMBALL_CAN_APP_H
#define GIMBALL_CAN_APP_H

#include "main.h"
#include "can_protocol.h"
#include "bsp_can.h"

#define CAN_SEND_PERIOD_MS 10U
#define CAN_HEARTBEAT_TIMEOUT 100U
#define CAN_TX_TIMEOUT 5U
#define WHEEL_MAX_RPM 200.0f

typedef struct {
    GimbalCtrlMsg_t gimbal_ctrl;
    uint8_t gimbal_heartbeat;
    ChassisFeedbackMsg_t chassis_feedback_rx;
    uint8_t chassis_fb_updated;
    uint8_t chassis_online;
    uint8_t last_chassis_hb;
    uint32_t last_chassis_rx_time;
    uint8_t can_comm_ok;
    uint32_t can_tx_cnt;
    uint32_t can_rx_cnt;
} CAN_AppState_t;

extern CAN_AppState_t g_can_state;

void CAN_App_Init(void);
uint8_t CAN_App_IsCommOK(void);
void CAN_App_SetGimbalCtrl(int16_t servo_speed, int16_t wheel_speed);
void CAN_App_SetStatusFlag(uint8_t flag, uint8_t enable);
ChassisFeedbackMsg_t CAN_App_GetChassisFeedback(void);
uint8_t CAN_App_IsChassisFeedbackUpdated(void);

#endif //GIMBALL_CAN_APP_H
