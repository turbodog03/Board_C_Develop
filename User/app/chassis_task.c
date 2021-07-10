#include "chassis_task.h"
#include "bsp_can.h"
#include "cmsis_os.h"

 

//#include "FreeRTOS.h"
//#include "task.h"

#include "sweep_wave.h"

int16_t current_tx[24995];
uint8_t data[8];
int16_t sent_current =0;
void chassis_task(void const *pvParameters){
	
	for(int i = 0;i<24995;i++)
	{
	current_tx[i] = 5*current[i];
	}


	uint32_t chassis_wake_time = osKernelSysTick();
//TickType_t lastWakeTime;
//const TickType_t frequency=500;
//lastWakeTime=xTaskGetTickCount();	
//	
	while(1){
//	vTaskDelayUntil(&lastWakeTime,frequency);
	static int count = 0;
	

  data[0] = current_tx[count] >> 8;
  data[1] = current_tx[count];
  data[2] = current_tx[count] >> 8;
  data[3] = current_tx[count];
  data[4] = current_tx[count] >> 8;
  data[5] = current_tx[count];
  data[6] = current_tx[count] >> 8;
  data[7] = current_tx[count];
	
	sent_current = current_tx[count];

		write_can(CHASSIS_CAN,CAN_CHASSIS_ID,data);

	if(count >= 24995){
		count = 0;
		}
	else{
		count++;
		}
	
	osDelayUntil(&chassis_wake_time, 10);
//	osThreadTerminate(NULL);
	}
	
}

