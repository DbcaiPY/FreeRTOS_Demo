
#include "FreeRTOS.h"         // ARM.FreeRTOS::RTOS:Core
#include "task.h"             // ARM.FreeRTOS::RTOS:Core
#include "cmsis_os.h"         // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"     // ARM.FreeRTOS::RTOS:Event Groups

#include "driver_oled.h"
#include "driver_key.h"
#include "driver_led.h"
#include "driver_oled.h"
#include "driver_timer.h"
#include "driver_lcd.h"
#include "driver_light_sensor.h"
#include "driver_dht11.h"

void Demo_Test(void *params)
{
		DHT11_Init();
		Key_Init();
		Led_Init();
		LightSensor_Init();
		OLED_Init();
		OLED_Clear();


		
	
		while(1)
		{
			
		}

}



