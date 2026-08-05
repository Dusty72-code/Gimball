//
// Created by Dolores on 2026/7/30.
//
#include "bsp_joystick.h"
#include <stdlib.h>

static uint16_t read_adc_channel(uint32_t channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = channel;
    sConfig.Rank = ADC_REGULAR_RANK_1;
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    HAL_ADC_Start(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 5) == HAL_OK) {
        return (uint16_t)HAL_ADC_GetValue(&hadc1);
    }
    return JOYSTICK_CENTER;
}

uint16_t BSP_Joystick_ReadX(void) {
    return read_adc_channel(ADC_CHANNEL_0);
}

uint16_t BSP_Joystick_ReadY(void) {
    return read_adc_channel(ADC_CHANNEL_1);
}

static int8_t adc_to_percent(uint16_t adc_val)
{
    int32_t val = (int32_t)adc_val;
    if (val > JOYSTICK_CENTER - JOYSTICK_DEADZONE && val < JOYSTICK_CENTER + JOYSTICK_DEADZONE) {
        return 0;
    }
    if (val <= JOYSTICK_CENTER) {
        int32_t range = JOYSTICK_CENTER - JOYSTICK_DEADZONE;
        if (range <= 0) return 0;
        return (int8_t)(-(JOYSTICK_CENTER - val) * 100 / range);
    }
    else {
        int32_t range = JOYSTICK_ADC_MAX - JOYSTICK_CENTER - JOYSTICK_DEADZONE;
        if (range <= 0) return 0;
        return (int8_t)((val - JOYSTICK_CENTER - JOYSTICK_DEADZONE) * 100 / range);
    }
}

int8_t BSP_Joystick_GetXPercent(void) {
    return adc_to_percent(BSP_Joystick_ReadX());
}

int8_t BSP_Joystick_GetYPercent(void) {
    return adc_to_percent(BSP_Joystick_ReadY());
}

uint8_t BSP_Joystick_IsSWPressed(void)
{
    static uint32_t last_press_time = 0U;
    static uint8_t first_done = 0U;
    static uint8_t last_state = 0U;
    uint8_t raw = (HAL_GPIO_ReadPin(SW_GPIO_Port, SW_Pin) == GPIO_PIN_RESET) ? 1U : 0U;
    if (raw != last_state) {
        last_state = raw;
        if (raw) {
            uint32_t now = HAL_GetTick();
            if (first_done && ((now - last_press_time) < 300U)) return 0U;
            first_done = 1U;
            last_press_time = now;
            return 1U;
        }
    }
    return 0U;
}
