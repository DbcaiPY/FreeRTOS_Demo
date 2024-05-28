// SPDX-License-Identifier: GPL-3.0-only
/*
 * Copyright (c) 2008-2023 100askTeam : Dongshan WEI <weidongshan@qq.com> 
 * Discourse:  https://forums.100ask.net
 */
 
/*  Copyright (C) 2008-2023 深圳百问网科技有限公司
 *  All rights reserved
 *
 * 免责声明: 百问网编写的文档, 仅供学员学习使用, 可以转发或引用(请保留作者信息),禁止用于商业用途！
 * 免责声明: 百问网编写的程序, 可以用于商业用途, 但百问网不承担任何后果！
 * 
 * 本程序遵循GPL V3协议, 请遵循协议
 * 百问网学习平台   : https://www.100ask.net
 * 百问网交流社区   : https://forums.100ask.net
 * 百问网官方B站    : https://space.bilibili.com/275908810
 * 本程序所用开发板 : DShanMCU-F103
 * 百问网官方淘宝   : https://100ask.taobao.com
 * 联系我们(E-mail): weidongshan@qq.com
 *
 *          版权所有，盗版必究。
 *  
 * 修改历史     版本号           作者        修改内容
 *-----------------------------------------------------
 * 2023.08.04      v01         百问科技      创建文件
 *-----------------------------------------------------
 */

#include "driver_light_sensor.h"
#include "driver_lcd.h"
#include "driver_timer.h"
#include "stm32f1xx_hal.h"
//#include "tim.h"
#include "adc.h"

#include "FreeRTOS.h"
#include "task.h"

#define LIGHTSENSOR_GPIO_GROUP      GPIOB
#define LIGHTSENSOR_GPIO_PIN        GPIO_PIN_11

#define LIGHT_SENSOR_ADC_TIMEOUT 500

extern ADC_HandleTypeDef hadc1;
static ADC_HandleTypeDef *g_HADC_LightSensor = &hadc1;

/**********************************************************************
 * 函数名称： LightSensor_Init
 * 功能描述： 光敏电阻的初始化函数
 * 输入参数： 无
 * 输出参数： 无
 * 返 回 值： 无
 * 修改日期：      版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2023/08/04	     V1.0	  韦东山	      创建
 ***********************************************************************/
void LightSensor_Init(void)
{
    /* PA3用作ADC(ADC1_IN3) */
    HAL_ADCEx_Calibration_Start(g_HADC_LightSensor);
	
    GPIO_InitTypeDef GPIO_InitStruct = {0};    
    GPIO_InitStruct.Pin  = LIGHTSENSOR_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(LIGHTSENSOR_GPIO_GROUP, &GPIO_InitStruct);
}

/**********************************************************************
 * 函数名称： LightSensor_Read
 * 功能描述： 读取光敏电阻的ADC值
 * 输入参数： 无
 * 输出参数： pData - 用来保存ADC值
 * 返 回 值： 0 - 成功, (-1) - 失败
 * 修改日期：      版本号     修改人	      修改内容
 * -----------------------------------------------
 * 2023/08/04	     V1.0	  韦东山	      创建
 ***********************************************************************/
int LightSensor_Read(uint32_t *pData)
{
     HAL_ADC_Start(g_HADC_LightSensor);
     if (HAL_OK == HAL_ADC_PollForConversion(g_HADC_LightSensor, LIGHT_SENSOR_ADC_TIMEOUT))
     {
        *pData = HAL_ADC_GetValue(g_HADC_LightSensor);
        return 0;
     }
     else
     {
        return -1;
     }
}

void LightSensor_State(uint8_t *pState)
{
		if (GPIO_PIN_SET == HAL_GPIO_ReadPin(LIGHTSENSOR_GPIO_GROUP, LIGHTSENSOR_GPIO_PIN))
    {
				*pState = 1;
		}
    else
		{
				*pState = 0;
		}
         
}

/**********************************************************************
 * 函数名称： LightSensor_Test
 * 功能描述： 光敏电阻测试程序
 * 输入参数： 无
 * 输出参数： 无
 *            无
 * 返 回 值： 无
 * 修改日期        版本号     修改人        修改内容
 * -----------------------------------------------
 * 2023/08/04        V1.0     韦东山       创建
 ***********************************************************************/
void LightSensor_Test(void)
{
    uint32_t analog_val;
    uint8_t val;
    LightSensor_Init();
		LCD_Clear();

    while (1)
    {
        
        LCD_PrintString(0, 0, "Light Sensor: ");

//        if (!LightSensor_Read(&analog_val))
//        {
//            LCD_PrintSignedVal(0, 2, analog_val);
//        }

				LightSensor_State(&val);
				
				//LCD_PrintSignedVal(0,2,val);
				LCD_PrintSignedVal(14,0,val);

        
        //mdelay(500);
				//vTaskDelay(500);
				HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
				vTaskDelay(500);
    }
}

