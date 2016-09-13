
#include "uart.h"
#include "stm32f1xx_hal.h"

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;


/* UART init function */
void UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 921600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
//  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
}

void UART_SendChar( char ch )
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 100);
}

