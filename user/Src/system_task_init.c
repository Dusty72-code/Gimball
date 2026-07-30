//
// Created by Dolores on 2026/7/30.
//
#include "system_task_init.h"
#include "can_app.h"
#include "bsp_can.h"
#include "LED.h"
#include "servo_control.h"

void System_Periph_Init(void)
{
    BSP_CAN_FilterInit();
    CAN_App_Init();
    LED_Status_Init();
    ServoControl_Init();
}