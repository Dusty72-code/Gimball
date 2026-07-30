//
// Created by Dolores on 2026/7/30.
//
#include "LED.h"

void LED_Status_Init(void){

    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}