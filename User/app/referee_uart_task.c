#include "referee_uart_task.h"
#include "can_device.h"
#include "bsp_uart.h"
#include "cmsis_os.h"
#include "chassis_task.h"

float a[4];
void referee_uart_task(){
		uint32_t uart_wake_time = osKernelSysTick();
		while(1){
		float a[] = {moto_chassis[0].speed_rpm, sent_current};
		HAL_UART_Transmit(&huart1,(uint8_t*)a,sizeof(a),HAL_MAX_DELAY);
//		write_uart(DEBUG_UART,(uint8_t*)a,sizeof(a));		
		uint8_t tail[]={0x00,0x00,0x80,0x7f};
//		write_uart(DEBUG_UART,tail,sizeof(tail));		
		HAL_UART_Transmit(&huart1,(uint8_t*)tail,sizeof(tail),HAL_MAX_DELAY);	
		
		osDelayUntil(&uart_wake_time, 5);
}
}
