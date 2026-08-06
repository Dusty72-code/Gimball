//
// Created by Dolores on 2026/7/30.
//
#include "LED.h"

void LED_Status_Init(void){
    /* 3 slow visible power-on blinks: ~300ms ON / ~200ms OFF, then stay ON */
    for (int i = 0; i < 3; i++) {
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        for (volatile uint32_t d = 0U; d < 21600000U; d++) { __NOP(); }
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
        for (volatile uint32_t d = 0U; d < 14400000U; d++) { __NOP(); }
    }
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}