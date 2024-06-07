/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "driver_key.h"
#include "driver_led.h"
#include "driver_oled.h"
#include "driver_timer.h"
#include "driver_lcd.h"
#include "driver_light_sensor.h"
#include "driver_dht11.h"
#include "driver_passive_buzzer.h"
#include "driver_active_buzzer.h"
#include "MyMax30102.h"

#include "MyTask.h"
#include "Data.h"
#include "ShowMenu.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

static BaseType_t keyTestRet;
static BaseType_t keyDealRet;
static BaseType_t lightSensorRet;
static BaseType_t dhtRet;
static BaseType_t createTaskRet;

static TaskHandle_t  xKeyDealHandle = NULL;


TimerHandle_t g_Timer;
TimerHandle_t g_Clock_Timer;

TaskHandle_t xRootTaskHandle = NULL;

TaskHandle_t xShowTimeTaskHandle = NULL;
TaskHandle_t xShowMenuTaskHandle = NULL;
TaskHandle_t xShowCalendarTaskHandle = NULL;
TaskHandle_t xShowClockTaskHandle = NULL;
TaskHandle_t xShowFlashLightTaskHandle = NULL;
TaskHandle_t xShowSettingTaskHandle = NULL;
TaskHandle_t xShowWoodenFishTaskHandle = NULL;
TaskHandle_t xShowDHT11TaskHandle = NULL;
TaskHandle_t xShowHRSPO2TaskHandle = NULL;

QueueHandle_t g_xQueueMenu;	
uint16_t key1_filter = 0;
uint16_t key2_filter = 0;
uint16_t key3_filter = 0;
uint16_t key4_filter = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

TaskHandle_t GetKeyDealHandle(void);

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	LCD_Init();
	LCD_Clear();
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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
	
	createTaskRet = xTaskCreate(MyTask_CreateTask, "CreateTask", 128, NULL, osPriorityNormal1, NULL);
	
	xTaskCreate(ShowMenuTask, "ShowMenuTask", 256, NULL, osPriorityNormal, &xShowMenuTaskHandle);
	
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	LCD_Init();
	LCD_Clear();
	
	for(;;)
	{
		//osDelay(1);
		//Led_Test();
		//LCD_Test();
		//HAL_Delay(200);
		//Key_Test();
		//LightSensor_Test();
		//DHT11_Test();
		//PassiveBuzzer_Test();
		//ActiveBuzzer_Test();
//		printf("hello!\r\n");
//		HAL_Delay(500);
//		Max30102_Test();
		
		osDelay(20);
				
		//vTaskDelete(defaultTaskHandle);
	}

  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{	
	/* key interrupt : send data to queue */
	
	/* some data maybe useless */
	extern BaseType_t end_flag;
	extern BaseType_t seclect_end;
	BaseType_t  RM_Flag, LM_Flag, EN_Flag, EX_Flag;
	Key_data key_data;
		
    if(GPIO_Pin == GPIO_PIN_11)
	{ 
		for(int i = 0; i<5000; i++){}
		if(end_flag == 1&&seclect_end == 0)
		{
			RM_Flag = 1;
			key_data.rdata = RM_Flag;
			xQueueSendToBackFromISR(g_xQueueMenu, &key_data, NULL);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			RM_Flag = 0;			
		}
	}
	if(GPIO_Pin == GPIO_PIN_10)
	{ 
		for(int i = 0; i<5000; i++){}
		if(end_flag == 1&&seclect_end == 0)
		{
		 	LM_Flag = 1;
			key_data.ldata = LM_Flag;
			xQueueSendToBackFromISR(g_xQueueMenu, &key_data, NULL);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			LM_Flag = 0;
		}
	}
	if(GPIO_Pin == GPIO_PIN_1)
	{
		for(int i = 0; i<5000; i++){}		
		if(end_flag == 1&&seclect_end == 0)
		{
			EN_Flag = 1;
			key_data.updata = EN_Flag;
			xQueueSendToBackFromISR(g_xQueueMenu, &key_data, NULL);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			EN_Flag = 0;
		}
	}
	if(GPIO_Pin == GPIO_PIN_0)
	{
		for(int i = 0; i<5000; i++){}		
		if(end_flag == 1&&seclect_end == 0)
		{
			EX_Flag = 1;
			key_data.exdata = EX_Flag;
			if(end_flag == 1&&seclect_end == 0)xQueueSendToBackFromISR(g_xQueueMenu, &key_data, NULL);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
			EX_Flag = 0;
		}
	}
}


/* USER CODE END Application */

