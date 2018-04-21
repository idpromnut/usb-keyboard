#include "usb_ctrl.h"
#include "stm32f1xx_hal.h"

void USB_Control_Init()
{
	// pull up the USB bus enable
	// Initialize the port and the pin
	GPIO_InitTypeDef   GPIO_InitStructure;
	GPIO_InitStructure.Pin = USB_ENABLE_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init( USB_ENABLE_PORT, &GPIO_InitStructure );
	USB_Control_Disable();
}

void USB_Control_Enable()
{
	HAL_GPIO_WritePin( USB_ENABLE_PORT, USB_ENABLE_PIN, GPIO_PIN_SET );
}

void USB_Control_Disable()
{
	HAL_GPIO_WritePin( USB_ENABLE_PORT, USB_ENABLE_PIN, GPIO_PIN_RESET );
}

void USB_Device_Connected()
{
}

void USB_Device_Disconnected()
{
}
