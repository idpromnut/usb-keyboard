#ifndef __UART_H__
#define __UART_H__

void UART_Init(UART_HandleTypeDef * huart);
void UART_SendChar(char c);

#endif /* __UART_H__ */
