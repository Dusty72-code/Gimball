//
// Created by Dolores on 2026/7/30.
//

#ifndef GIMBALL_SERVO_CONTROL_H
#define GIMBALL_SERVO_CONTROL_H

#include "bsp_servo.h"

#define SERVO_MAX_SPEED_RPM 60
#define SERVO_CTRL_PERIOD_MS 10U

typedef struct {
    int8_t target_speed;
    int16_t target_rpm;
    uint8_t servo_online;
} ServoCtrl_t;

extern ServoCtrl_t g_servo;

void ServoControl_Init(void);
void ServoControl_SetSpeed(int8_t speed_percent);

#endif //GIMBALL_SERVO_CONTROL_H
