//
// Created by Dolores on 2026/7/30.
//
#include "servo_control.h"
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

ServoCtrl_t g_servo;

void ServoControl_Init(void)
{
    memset(&g_servo, 0, sizeof(g_servo));
    g_servo.servo_online = 1U;
    BSP_Servo_Init();
}

void ServoControl_SetSpeed(int8_t speed_percent)
{
    taskENTER_CRITICAL();
    g_servo.target_speed = speed_percent;
    g_servo.target_rpm = (int16_t)((float)speed_percent / 100.0f * (float)SERVO_MAX_SPEED_RPM);
    taskEXIT_CRITICAL();
}