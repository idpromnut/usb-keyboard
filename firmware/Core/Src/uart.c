
#include "main.h"
#include "uart.h"
#include "stm32f1xx_hal.h"

#define UART_BAUD_RATE	(DEBUG_BAUD_RATE)

/* Private variables ---------------------------------------------------------*/
static UART_HandleTypeDef * uart;

void UART_Init(UART_HandleTypeDef * huart)
{
	uart = huart;
}

void UART_SendChar( char ch )
{
	HAL_UART_Transmit(uart, (uint8_t*)&ch, 1, 100);
}
