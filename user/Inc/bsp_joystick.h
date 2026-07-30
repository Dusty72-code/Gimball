//
// Created by Dolores on 2026/7/30.
//

#ifndef GIMBALL_BSP_JOYSTICK_H
#define GIMBALL_BSP_JOYSTICK_H

#include "main.h"
#include "adc.h"

#define JOYSTICK_ADC_MAX 4095U
#define JOYSTICK_CENTER 2048U
#define JOYSTICK_DEADZONE 200U
#define JOYSTICK_SAMPLE_PERIOD_MS 10U

uint16_t BSP_Joystick_ReadX(void);
uint16_t BSP_Joystick_ReadY(void);
int8_t BSP_Joystick_GetXPercent(void);
int8_t BSP_Joystick_GetYPercent(void);
uint8_t BSP_Joystick_IsSWPressed(void);

#endif //GIMBALL_BSP_JOYSTICK_H
