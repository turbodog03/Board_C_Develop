
#include "bsp_uart.h"

/**
  * @brief     发送 UART 数据
  * @param     uart_id: UART ID
  * @param     send_data: 发送数据指针
  * @param     size: 发送数据的长度
  */
void write_uart(UART_HandleTypeDef *huart, uint8_t *pData,uint16_t size){
		HAL_UART_Transmit_DMA(huart, pData, size);
}


//如果需要写收发数据的回调函数再在.c里自定义
//只有DEBUG_UART（USART1）开了全局中断
void  HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
;
}



void  HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
;
}

