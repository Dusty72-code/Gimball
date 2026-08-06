//
// Created by Dolores on 2026/7/30.
//
#include "LED.h"
#include "cmsis_os.h"

void LED_Status_Init(void){
    /* Early boot: just turn the LED on. The visible power-on blink is now
       performed inside the LED task via LED_StartBlink() using vTaskDelay,
       so it no longer blocks the scheduler start-up. */
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

void LED_StartBlink(void){
    /* 3 visible power-on blinks: ~300ms ON / ~200ms OFF each, then stay ON.
       Called from the LED task, so vTaskDelay is used instead of NOP spin. */
    for (int i = 0; i < 3; i++) {
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(300));
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}