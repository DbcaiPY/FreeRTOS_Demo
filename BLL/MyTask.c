#include "MyInclude.h"


TaskHandle_t xKeyDealHandle;
TaskHandle_t xKeyTestHandle;
TaskHandle_t xLightSensorHandle;
TaskHandle_t xDHT11Handle;

void MyTask_PeriphInit(void)
{
	
}

void MyTask_CreateTask(void *argument)
{
		
	size_t heapsize;
	
	xTaskCreate(Key_Test, "KeyTask", 128, NULL, osPriorityNormal2, &xKeyTestHandle);	
	xTaskCreate(Key_Deal, "KeyDealTask", 128, NULL, osPriorityNormal1, &xKeyDealHandle);
	xTaskCreate(LightSensor_Test, "LightSensorTask", 128, NULL, osPriorityNormal1, &xLightSensorHandle);
	xTaskCreate(DHT11_Test, "DHt11Task", 128, NULL, osPriorityNormal1, &xDHT11Handle);

	for(;;)
	{
		heapsize = xPortGetFreeHeapSize();
		LCD_PrintSignedVal(0, 6, heapsize);
		vTaskDelay(1000);

	}
	
}

