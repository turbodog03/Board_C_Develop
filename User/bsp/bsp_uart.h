
#ifndef BSP_BSP_UART_H
#define BSP_BSP_UART_H
//bluetooth_debug

#include "main.h"
#include "usart.h"


extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
/*
	uart��;����
*/
#define DEBUG_UART  &huart1
#define DBUS_UART  &huart3
#define REFEREE_UART  &huart6

//�õ�dma������ֻ��DEBUG_UART����dma����
extern void write_uart(UART_HandleTypeDef *huart, uint8_t *pData,uint16_t size);



#endif //BSP_BSP_UART_DEBUG_H



