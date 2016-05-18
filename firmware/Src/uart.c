
#include "uart.h"
#include "stm32f1xx_hal.h"

/* Private variables ---------------------------------------------------------*/
USART_HandleTypeDef huart1;


/* UART init function */
void UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = USART_WORDLENGTH_8B;
  huart1.Init.StopBits = USART_STOPBITS_1;
  huart1.Init.Parity = USART_PARITY_NONE;
  huart1.Init.Mode = USART_MODE_TX_RX;
//  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_USART_Init(&huart1);
}

void UART_SendChar( char ch )
{
	HAL_USART_Transmit(&huart1, (uint8_t*)&ch, 1, 100);
}

