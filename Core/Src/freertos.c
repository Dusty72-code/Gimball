/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "bsp_can.h"
#include "bsp_joystick.h"
#include "bsp_servo.h"
#include "can_app.h"
#include "can_protocol.h"
#include "LED.h"
#include "servo_control.h"
#include "system_task_init.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define CAN_HEARTBEAT_MASK 0xFFU
#define SELF_TEST_PHASE_DURATION_MS 1000U

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
static uint8_t g_self_test_active = 0U;
static const uint8_t breath_table[LED_PWM_RESOLUTION] = {
  0,  3,  6,  9, 12, 15, 18, 21, 24, 27,
 30, 33, 36, 39, 42, 45, 48, 50, 53, 55,
 58, 60, 63, 65, 67, 69, 71, 73, 75, 76,
 78, 79, 80, 81, 82, 83, 84, 84, 85, 85,
 86, 86, 86, 86, 86, 86, 86, 85, 85, 84,
 84, 83, 82, 81, 80, 79, 78, 76, 75, 73,
 71, 69, 67, 65, 63, 60, 58, 55, 53, 50,
 48, 45, 42, 39, 36, 33, 30, 27, 24, 21,
 18, 15, 12,  9,  6,  3,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};

/* USER CODE END Variables */
/* Definitions for CAN_Send */
osThreadId_t CAN_SendHandle;
const osThreadAttr_t CAN_Send_attributes = {
  .name = "CAN_Send",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CAN_Recv */
osThreadId_t CAN_RecvHandle;
const osThreadAttr_t CAN_Recv_attributes = {
  .name = "CAN_Recv",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CAN_HB */
osThreadId_t CAN_HBHandle;
const osThreadAttr_t CAN_HB_attributes = {
  .name = "CAN_HB",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Joystick */
osThreadId_t JoystickHandle;
const osThreadAttr_t Joystick_attributes = {
  .name = "Joystick",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Servo */
osThreadId_t ServoHandle;
const osThreadAttr_t Servo_attributes = {
  .name = "Servo",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LED */
osThreadId_t LEDHandle;
const osThreadAttr_t LED_attributes = {
  .name = "LED",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void SelfTest_Execute(void);
/* USER CODE END FunctionPrototypes */

void StartCAN_SendTask(void *argument);
void StartCAN_RecvTask(void *argument);
void StartCAN_HBTask(void *argument);
void StartJoystickTask(void *argument);
void StartServoTask(void *argument);
void StartLEDTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of CAN_Send */
  CAN_SendHandle = osThreadNew(StartCAN_SendTask, NULL, &CAN_Send_attributes);

  /* creation of CAN_Recv */
  CAN_RecvHandle = osThreadNew(StartCAN_RecvTask, NULL, &CAN_Recv_attributes);

  /* creation of CAN_HB */
  CAN_HBHandle = osThreadNew(StartCAN_HBTask, NULL, &CAN_HB_attributes);

  /* creation of Joystick */
  JoystickHandle = osThreadNew(StartJoystickTask, NULL, &Joystick_attributes);

  /* creation of Servo */
  ServoHandle = osThreadNew(StartServoTask, NULL, &Servo_attributes);

  /* creation of LED */
  LEDHandle = osThreadNew(StartLEDTask, NULL, &LED_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartCAN_SendTask */
/**
  * @brief  Function implementing the CAN_Send thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartCAN_SendTask */
void StartCAN_SendTask(void *argument)
{
  /* USER CODE BEGIN StartCAN_SendTask */
  (void)argument;
  uint8_t tx_data[8];
  TickType_t xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CAN_SEND_PERIOD_MS));
    taskENTER_CRITICAL();
    g_can_state.gimbal_ctrl.gimbal_heartbeat = g_can_state.gimbal_heartbeat;
    GimbalCtrlMsg_t ctrl = g_can_state.gimbal_ctrl;
    taskEXIT_CRITICAL();
    Protocol_EncodeGimbalCtrl(&ctrl, tx_data);
    HAL_StatusTypeDef status = BSP_CAN_SendMessage(CAN_GIMBAL_TO_CHASSIS_ID, tx_data, CAN_TX_TIMEOUT);
    taskENTER_CRITICAL();
    g_can_state.gimbal_heartbeat = (g_can_state.gimbal_heartbeat + 1U) & CAN_HEARTBEAT_MASK;
    taskEXIT_CRITICAL();
    if (status == HAL_OK) g_can_state.can_tx_cnt++;
    osDelay(1);
  }
  /* USER CODE END StartCAN_SendTask */
}

/* USER CODE BEGIN Header_StartCAN_RecvTask */
/**
* @brief Function implementing the CAN_Recv thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCAN_RecvTask */
void StartCAN_RecvTask(void *argument)
{
  /* USER CODE BEGIN StartCAN_RecvTask */
  (void)argument;
  BSP_CAN_RxMsg_t rx_msg;
  /* Infinite loop */
  for(;;)
  {
    if (BSP_CAN_GetRxMessage(&rx_msg) == 0U) {
      vTaskDelay(pdMS_TO_TICKS(1));
      continue;
    }
    if (rx_msg.header.StdId == CAN_CHASSIS_TO_GIMBAL_ID) {
      taskENTER_CRITICAL();
      Protocol_DecodeChassisFeedback(rx_msg.data, &g_can_state.chassis_feedback_rx);
      g_can_state.chassis_fb_updated = 1U;
      g_can_state.last_chassis_rx_time = HAL_GetTick();
      g_can_state.can_rx_cnt++;
      if (g_can_state.chassis_feedback_rx.chassis_heartbeat != g_can_state.last_chassis_hb) {
        g_can_state.last_chassis_hb = g_can_state.chassis_feedback_rx.chassis_heartbeat;
        g_can_state.chassis_online = 1U;
      }
      taskEXIT_CRITICAL();
    }
    osDelay(1);
  }
  /* USER CODE END StartCAN_RecvTask */
}

/* USER CODE BEGIN Header_StartCAN_HBTask */
/**
* @brief Function implementing the CAN_HB thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartCAN_HBTask */
void StartCAN_HBTask(void *argument)
{
  /* USER CODE BEGIN StartCAN_HBTask */
  (void)argument;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  vTaskDelay(pdMS_TO_TICKS(500));
  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(50));
    uint32_t now = HAL_GetTick();
    uint8_t prev_comm_ok = g_can_state.can_comm_ok;
    uint8_t comm_ok = 1U;
    if ((now - g_can_state.last_chassis_rx_time) > CAN_HEARTBEAT_TIMEOUT) {
      g_can_state.chassis_online = 0U;
      comm_ok = 0U;
    }
    g_can_state.can_comm_ok = comm_ok;
    if (prev_comm_ok != comm_ok) {
      taskENTER_CRITICAL();
      if (comm_ok)
        g_can_state.gimbal_ctrl.status_flags &= ~STATUS_CAN_ERROR;
      else
        g_can_state.gimbal_ctrl.status_flags |= STATUS_CAN_ERROR;
      taskEXIT_CRITICAL();
    }
    osDelay(1);
  }
  /* USER CODE END StartCAN_HBTask */
}

/* USER CODE BEGIN Header_StartJoystickTask */
/**
* @brief Function implementing the Joystick thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartJoystickTask */
void StartJoystickTask(void *argument)
{
  /* USER CODE BEGIN StartJoystickTask */
  (void)argument;
  /* Calibrate joystick centre (keep sticks untouched during power-up) */
  BSP_Joystick_Calibrate();
  TickType_t xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(JOYSTICK_SAMPLE_PERIOD_MS));
    if (!g_self_test_active && BSP_Joystick_IsSWPressed()) {
      g_self_test_active = 1U;
      SelfTest_Execute();
      g_self_test_active = 0U;
      continue;
    }
    if (g_self_test_active) {
      continue;
    }
    int8_t x_percent = BSP_Joystick_GetXPercent();
    int8_t y_percent = BSP_Joystick_GetYPercent();
    int16_t servo_rpm = (int16_t)((float)x_percent / 100.0f * SERVO_MAX_SPEED_RPM);
    int16_t wheel_rpm = (int16_t)((float)y_percent / 100.0f * WHEEL_MAX_RPM);
    ServoControl_SetSpeed(x_percent);
    CAN_App_SetGimbalCtrl(servo_rpm, wheel_rpm);
    osDelay(1);
  }
  /* USER CODE END StartJoystickTask */
}

/* USER CODE BEGIN Header_StartServoTask */
/**
* @brief Function implementing the Servo thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartServoTask */
void StartServoTask(void *argument)
{
  /* USER CODE BEGIN StartServoTask */
  (void)argument;
  TickType_t xLastWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SERVO_CTRL_PERIOD_MS));
    int8_t speed = g_servo.target_speed;
    BSP_Servo_SetSpeed(speed);
    osDelay(1);
  }
  /* USER CODE END StartServoTask */
}

/* USER CODE BEGIN Header_StartLEDTask */
/**
* @brief Function implementing the LED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLEDTask */
void StartLEDTask(void *argument)
{
  /* USER CODE BEGIN StartLEDTask */
  (void)argument;
  uint8_t breath_idx = 0U;
  uint8_t should_breath = 0U;
  /* Infinite loop */
  for(;;)
  {
    if (g_self_test_active) {
      vTaskDelay(pdMS_TO_TICKS(50));
      continue;
    }
    should_breath = g_can_state.can_comm_ok ? 0U : 1U;
    if (should_breath) {
      /* software PWM breathing using breath_table */
      uint8_t level = breath_table[breath_idx];
      uint32_t on_ms  = (uint32_t)level * LED_BREATH_STEP_MS / 100U;
      uint32_t off_ms = LED_BREATH_STEP_MS - on_ms;
      if (on_ms > 0U) {
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
        vTaskDelay(pdMS_TO_TICKS(on_ms));
      }
      if (off_ms > 0U) {
        HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
        vTaskDelay(pdMS_TO_TICKS(off_ms));
      }
      breath_idx++;
      if (breath_idx >= LED_PWM_RESOLUTION) breath_idx = 0U;
    } else {
      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
      vTaskDelay(pdMS_TO_TICKS(200));
      breath_idx = 0U;
    }
    osDelay(1);
  }
  /* USER CODE END StartLEDTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static void SelfTest_Execute(void)
{
  CAN_App_SetStatusFlag(STATUS_SELF_TEST, 1U);
  for (int i = 0; i < 3; i++) {
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
    vTaskDelay(pdMS_TO_TICKS(150));
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
    vTaskDelay(pdMS_TO_TICKS(150));
  }
  CAN_App_SetGimbalCtrl(-60, 0);
  BSP_Servo_SetSpeed(-80);
  vTaskDelay(pdMS_TO_TICKS(SELF_TEST_PHASE_DURATION_MS));
  BSP_Servo_SetSpeed(80);
  vTaskDelay(pdMS_TO_TICKS(SELF_TEST_PHASE_DURATION_MS));
  BSP_Servo_SetSpeed(0);
  vTaskDelay(pdMS_TO_TICKS(500));
  CAN_App_SetGimbalCtrl(0, 100);
  vTaskDelay(pdMS_TO_TICKS(SELF_TEST_PHASE_DURATION_MS));
  CAN_App_SetGimbalCtrl(0, -100);
  vTaskDelay(pdMS_TO_TICKS(SELF_TEST_PHASE_DURATION_MS));
  CAN_App_SetGimbalCtrl(0, 0);
  CAN_App_SetStatusFlag(STATUS_SELF_TEST, 0U);
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

/* USER CODE END Application */

