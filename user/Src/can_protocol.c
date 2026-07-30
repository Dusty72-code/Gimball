//
// Created by Dolores on 2026/7/30.
//
#include "can_protocol.h"
#include <string.h>

void Protocol_EncodeGimbalCtrl(const GimbalCtrlMsg_t *msg, uint8_t data[8])
{
    if (msg == NULL || data == NULL) return;
    data[0] = (uint8_t)((msg->servo_target_speed >> 8) & 0xFF);
    data[1] = (uint8_t)( msg->servo_target_speed & 0xFF);
    data[2] = (uint8_t)((msg->wheel_target_speed >> 8) & 0xFF);
    data[3] = (uint8_t)( msg->wheel_target_speed & 0xFF);
    data[4] = msg->servo_online;
    data[5] = msg->gimbal_heartbeat;
    data[6] = msg->status_flags;
    data[7] = msg->reserved;
}

void Protocol_DecodeGimbalCtrl(const uint8_t data[8], GimbalCtrlMsg_t *msg)
{
    if (data == NULL || msg == NULL) return;
    msg->servo_target_speed = (int16_t)(((uint16_t)data[0] << 8) | data[1]);
    msg->wheel_target_speed = (int16_t)(((uint16_t)data[2] << 8) | data[3]);
    msg->servo_online = data[4];
    msg->gimbal_heartbeat = data[5];
    msg->status_flags = data[6];
    msg->reserved = data[7];
}

void Protocol_EncodeChassisFeedback(const ChassisFeedbackMsg_t *msg, uint8_t data[8])
{
    if (msg == NULL || data == NULL) return;
    data[0] = (uint8_t)((msg->motor_actual_speed >> 8) & 0xFF);
    data[1] = (uint8_t)( msg->motor_actual_speed & 0xFF);
    data[2] = (uint8_t)((msg->motor_encoder_raw >> 8) & 0xFF);
    data[3] = (uint8_t)( msg->motor_encoder_raw & 0xFF);
    data[4] = msg->motor_online;
    data[5] = msg->chassis_heartbeat;
    data[6] = msg->status_flags;
    data[7] = msg->reserved;
}

void Protocol_DecodeChassisFeedback(const uint8_t data[8], ChassisFeedbackMsg_t *msg)
{
    if (data == NULL || msg == NULL) return;
    msg->motor_actual_speed = (int16_t)(((uint16_t)data[0] << 8) | data[1]);
    msg->motor_encoder_raw = (int16_t)(((uint16_t)data[2] << 8) | data[3]);
    msg->motor_online = data[4];
    msg->chassis_heartbeat = data[5];
    msg->status_flags = data[6];
    msg->reserved = data[7];
}
