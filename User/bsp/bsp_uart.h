
#ifndef BSP_BSP_UART_H
#define BSP_BSP_UART_H
//bluetooth_debug

#include "main.h"
#include "usart.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
/*
	uart用途定义
*/
#define DEBUG_UART  &huart1
#define DBUS_UART  &huart3
#define REFEREE_UART  &huart6

//用的dma，不过只有DEBUG_UART开了dma发送
extern void write_uart(UART_HandleTypeDef *huart, uint8_t *pData,uint16_t size);



#endif //BSP_BSP_UART_DEBUG_H



