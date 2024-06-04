
#include "driver_key.h"
#include "driver_led.h"
#include "driver_lcd.h"
#include "driver_sg90.h"
#include "driver_oled.h"

#include "gpio.h"
#include "tim.h"

#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

static uint8_t Angle;

void SG90_Init(void)
{
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);	
}

void SG90_Close(void)
{
	for(Angle = 0; Angle <= 100; Angle++)
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, Angle);
		HAL_Delay(10);
	}	
}	

void SG90_Open(void)
{
	for(Angle = 0; Angle <= 100; Angle++)
	{
		__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 100-Angle);
		HAL_Delay(10);
	}	
}

void SG90_Test(void)
{
	SG90_Init();
	
	uint16_t i;
	
	for(;;)
	{
		for(i = 0; i <= 100; i++)
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, i);
			HAL_Delay(10);
		}
		for(i = 0; i <= 100; i++)
		{
			__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 100-i);
			HAL_Delay(10);
		}
	}
}


