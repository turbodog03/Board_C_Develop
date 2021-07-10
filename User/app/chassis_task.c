#include "chassis_task.h"
#include "bsp_can.h"
#include "cmsis_os.h"

void chassis_task(){
	uint32_t chassis_wake_time = osKernelSysTick();
	
	static uint8_t data[8];
	write_can(CHASSIS_CAN,CAN_CHASSIS_ID,data);
	
	osDelayUntil(&chassis_wake_time, 10);
}

