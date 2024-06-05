#include "algorithm_max30102.h"
#include "driver_max30102.h"
#include <stdio.h>

#define MAX_BRIGHTNESS 255

#define INT_GPIO_Port 	GPIOB
#define INT_Pin       	GPIO_PIN_9

#define LED_GPIO_Port   GPIOA
#define LED_Pin         GPIO_PIN_2

uint32_t aun_ir_buffer[500];    //IR LED sensor data
int32_t n_ir_buffer_length;     //data length
uint32_t aun_red_buffer[500];   //Red LED sensor data

int32_t n_sp02;                 //SPO2 value
int8_t ch_spo2_valid;            //indicator to show if the SP02 calculation is valid 指示SP02计算是否有效的指示器

int32_t n_heart_rate;           //heart rate value 心率值
int8_t ch_hr_valid;             //indicator to show if the heart rate calculation is valid显示心率计算是否有效的指示器

uint8_t uch_dummy;

uint32_t un_min, un_max, un_prev_data;
int i;
int32_t n_brightness;
float f_temp;


void Max30102_Test(void)
{
	maxim_max30102_reset(); //resets the MAX30102
	
	//read and clear status register
	maxim_max30102_read_reg(0,&uch_dummy);

	maxim_max30102_init();  //initializes the MAX30102
	printf("初始化完成 \r\n");   
			
	n_brightness=0;
	
	un_min=0x3FFFF;
	
	un_max=0;

	n_ir_buffer_length=500; //buffer length of 100 stores 5 seconds of samples running at 100sps
	
	printf("进入循环 \r\n");
	//read the first 500 samples, and determine the signal range
	for(i=0;i<n_ir_buffer_length;i++)
	{
		// while(INT.read()==1);   //wait until the interrupt pin asserts

		while(HAL_GPIO_ReadPin(INT_GPIO_Port,INT_Pin)==1){};   //wait until the interrupt pin asserts

		maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));  //read from MAX30102 FIFO

		if(un_min>aun_red_buffer[i])
			un_min=aun_red_buffer[i];    //update signal min
		
		if(un_max<aun_red_buffer[i])
			un_max=aun_red_buffer[i];    //update signal max
		
//		printf("red=");
//		
//		printf("%i", aun_red_buffer[i]);
//		
//		printf(", ir=");
//		
//		printf("%i \r\n", aun_ir_buffer[i]);
	}
	un_prev_data=aun_red_buffer[i];
	
	printf("循环完成 \r\n");
    
	//calculate heart rate and SpO2 after first 500 samples (first 5 seconds of samples)
	maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	
	printf("开始主循环 \r\n");

	
	while(1)
	{
		i=0;

		un_min=0x3FFFF;
		
		un_max=0;

		//dumping the first 100 sets of samples in the memory and shift the last 400 sets of samples to the top
		for(i=100;i<500;i++)
		{
		aun_red_buffer[i-100]=aun_red_buffer[i];
				
			aun_ir_buffer[i-100]=aun_ir_buffer[i];

			//update the signal min and max
			if(un_min>aun_red_buffer[i])
				
			un_min=aun_red_buffer[i];
			
			if(un_max<aun_red_buffer[i])
				un_max=aun_red_buffer[i];
		}

		//take 100 sets of samples before calculating the heart rate.
		for(i=400;i<500;i++)
		{
			un_prev_data=aun_red_buffer[i-1];
			//while(INT.read()==1);
			while(HAL_GPIO_ReadPin(INT_GPIO_Port,INT_Pin)==1){};   //wait until the interrupt pin asserts

			maxim_max30102_read_fifo((aun_red_buffer+i), (aun_ir_buffer+i));

			if(aun_red_buffer[i]>un_prev_data)//just to determine the brightness of LED according to the deviation of adjacent two AD data
			{
				f_temp=aun_red_buffer[i]-un_prev_data;
				
				f_temp/=(un_max-un_min);
				
				f_temp*=MAX_BRIGHTNESS;
				
				n_brightness-=(int)f_temp;
				
				if(n_brightness<0)
					n_brightness=0;
			}
			else
			{
				f_temp=un_prev_data-aun_red_buffer[i];
				
				f_temp/=(un_max-un_min);
				
				f_temp*=MAX_BRIGHTNESS;
				
				n_brightness+=(int)f_temp;
				
				if(n_brightness>MAX_BRIGHTNESS)
					n_brightness=MAX_BRIGHTNESS;
			}

			// pwmled.write(1-(float)n_brightness/256);//pwm control led brightness
			if(n_brightness<120)	
			{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
			}
			//send samples and calculation result to terminal program through UART
//			printf("red=");
//			
//			printf("%i", aun_red_buffer[i]);
//			
//			printf(", ir=");
//			
//			printf("%i", aun_ir_buffer[i]);
			
//			printf("HR=%i, ", n_heart_rate); 
//			
//			printf("HRvalid=%i, ", ch_hr_valid);
//			
//			printf("SpO2=%i, ", n_sp02);
//			
//			printf("SPO2Valid=%i\n\r\n\r", ch_spo2_valid);
		}
		maxim_heart_rate_and_oxygen_saturation(aun_ir_buffer, n_ir_buffer_length, aun_red_buffer, &n_sp02, &ch_spo2_valid, &n_heart_rate, &ch_hr_valid); 
	
				
		printf("HR_AVER=%i , ", n_heart_rate);
		printf("HRvalid=%i , ", ch_hr_valid);
		printf("SpO2_AVER=%i , ", n_sp02);
		printf("SPO2Valid=%i\r\n", ch_spo2_valid);
	
	
	}
}





