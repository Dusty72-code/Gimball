//
// Created by Dolores on 2026/7/30.
//

#ifndef GIMBALL_BSP_SERVO_H
#define GIMBALL_BSP_SERVO_H

#include "main.h"
#include "tim.h"

#define SERVO_PULSE_STOP 1500U
#define SERVO_PULSE_MIN 500U
#define SERVO_PULSE_MAX 2500U

void BSP_Servo_Init(void);
void BSP_Servo_SetPulse(uint16_t pulse_us);
void BSP_Servo_SetSpeed(int8_t speed);
void BSP_Servo_Stop(void);

#endif //GIMBALL_BSP_SERVO_H
