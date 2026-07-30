//
// Created by Dolores on 2026/7/30.
//
#include "can_app.h"
#include "cmsis_os.h"
#include <string.h>

CAN_AppState_t g_can_state;

void CAN_App_Init(void)
{
    memset(&g_can_state, 0, sizeof(g_can_state));
    g_can_state.can_comm_ok = 0U;
    uint32_t now = HAL_GetTick();
    g_can_state.last_chassis_rx_time = now;
    g_can_state.gimbal_ctrl.status_flags = STATUS_SYSTEM_OK;
}

void CAN_App_SetGimbalCtrl(int16_t servo_speed, int16_t wheel_speed)
{
    taskENTER_CRITICAL();
    g_can_state.gimbal_ctrl.servo_target_speed = servo_speed;
    g_can_state.gimbal_ctrl.wheel_target_speed = wheel_speed;
    g_can_state.gimbal_ctrl.servo_online = 1U;
    taskEXIT_CRITICAL();
}

void CAN_App_SetStatusFlag(uint8_t flag, uint8_t enable)
{
    taskENTER_CRITICAL();
    if (enable) g_can_state.gimbal_ctrl.status_flags |= flag;
    else g_can_state.gimbal_ctrl.status_flags &= ~flag;
    taskEXIT_CRITICAL();
}

ChassisFeedbackMsg_t CAN_App_GetChassisFeedback(void)
{
    ChassisFeedbackMsg_t fb;
    taskENTER_CRITICAL();
    fb = g_can_state.chassis_feedback_rx;
    g_can_state.chassis_fb_updated = 0U;
    taskEXIT_CRITICAL();
    return fb;
}

uint8_t CAN_App_IsChassisFeedbackUpdated(void)
{
    return g_can_state.chassis_fb_updated;
}

uint8_t CAN_App_IsCommOK(void)
{
    return g_can_state.can_comm_ok;
}
