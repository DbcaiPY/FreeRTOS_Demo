#include "MyInclude.h"

TaskHandle_t xKeyDealHandle;
TaskHandle_t xKeyTestHandle;
TaskHandle_t xLightSensorHandle;
TaskHandle_t xDHT11Handle;
TaskHandle_t xBuzzerHandle;

void MyTask_PeriphInit(void)
{
	SG90_Init();
}

void MyTask_CreateTask(void *argument)
{
		
	MyTask_PeriphInit();
	
	size_t heapsize;
	
//	xTaskCreate(Key_Test, "KeyTask", 128, NULL, osPriorityNormal2, &xKeyTestHandle);	
//	xTaskCreate(Key_Deal, "KeyDealTask", 128, NULL, osPriorityNormal1, &xKeyDealHandle);
//	xTaskCreate(LightSensor_Test, "LightSensorTask", 128, NULL, osPriorityNormal1, &xLightSensorHandle);
//	xTaskCreate(DHT11_Test, "DHt11Task", 128, NULL, osPriorityNormal1, &xDHT11Handle);
//	xTaskCreate(PassiveBuzzer_Test, "DHt11Task", 128, NULL, osPriorityNormal1, &xBuzzerHandle);
//	xTaskCreate(SG90_Test, "SG90Task", 128, NULL, osPriorityNormal1, &xLightSensorHandle);

	
	
	for(;;)
	{
		
//		SG90_Open();
//		SG90_Close();
		heapsize = xPortGetFreeHeapSize();
		LCD_PrintSignedVal(0, 6, heapsize);
			
		vTaskDelay(1000);

	}
	
}

