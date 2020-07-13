#include "main.h"
#include "usb_ctrl.h"
#include "stm32f1xx_hal.h"

void USB_Control_Init()
{
	// pull up the USB bus enable
	// Initialize the port and the pin
	GPIO_InitTypeDef   GPIO_InitStructure;
	GPIO_InitStructure.Pin = USB_EN_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init( USB_EN_GPIO_Port, &GPIO_InitStructure );
	USB_Control_Disable();
}

void USB_Control_Enable()
{
	HAL_GPIO_WritePin( USB_EN_GPIO_Port, USB_EN_Pin, GPIO_PIN_SET );
}

void USB_Control_Disable()
{
	HAL_GPIO_WritePin( USB_EN_GPIO_Port, USB_EN_Pin, GPIO_PIN_RESET );
}

void USB_Device_Connected()
{
}

void USB_Device_Disconnected()
{
}
