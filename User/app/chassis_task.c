#include "chassis_task.h"
#include "bsp_can.h"
#include "cmsis_os.h"

 

//#include "FreeRTOS.h"
//#include "task.h"

//#include "sweep_wave.h"
//#include "sweep_wave_250_50s_0_6Hz.h"
//#include "sweep_wave_250_50s_0_6Hz_5ms.h"
#include "sweep_wave_250_50s_0_6Hz_3.h"
int max_index = sizeof(current)/sizeof(float) ;
uint8_t data[8];
int16_t sent_current =0;


void chassis_task(void const *pvParameters){
	
//	for(int i = 0;i<max_index-1;i++)
//	{
//	current_tx[i] = current[i];
//	}
		current[max_index] = 0;

	uint32_t chassis_wake_time = osKernelSysTick();
//TickType_t lastWakeTime;
//const TickType_t frequency=500;
//lastWakeTime=xTaskGetTickCount();	
//	
	while(1){
//	vTaskDelayUntil(&lastWakeTime,frequency);
	static int count = 0;
	

  data[0] = (int16_t) (1*current[count]) >> 8;
  data[1] = (int16_t) (1*current[count]);
  data[2] = (int16_t) (1*current[count])>> 8;
  data[3] = (int16_t) (1*current[count]);
  data[4] = (int16_t) (1*current[count])>> 8;
  data[5] = (int16_t) (1*current[count]);
  data[6] = (int16_t) (1*current[count]) >> 8;
  data[7] = (int16_t) (1*current[count]);
	
	sent_current = (1*current[count]);

		write_can(CHASSIS_CAN,CAN_CHASSIS_ID,data);

	if(count < max_index){
		count++;
		}
	else{
		count = max_index;
		}
	
	osDelayUntil(&chassis_wake_time, 5);
//	osThreadTerminate(NULL);
	}
	
}

