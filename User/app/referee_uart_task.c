#include "referee_uart_task.h"
#include "can_device.h"
#include "bsp_uart.h"
#include "cmsis_os.h"

void referee_uart_task(){
		uint32_t uart_wake_time = osKernelSysTick();
	
		float a[3] = {(float)moto_chassis[0].speed_rpm, moto_chassis[1].speed_rpm, moto_chassis[2].speed_rpm};
		write_uart(DEBUG_UART,(uint8_t*)a,sizeof(a));		
		uint8_t tail[]={0x00,0x00,0x80,0x7f};
		write_uart(DEBUG_UART,tail,sizeof(tail));		

		
		
		osDelayUntil(&uart_wake_time, 10);
}
