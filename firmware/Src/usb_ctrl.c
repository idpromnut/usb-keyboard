
#include "usb_ctrl.h"
#include "stm32f1xx_hal.h"
#include "user_interface.h"

void USB_Control_Init()
{
	// pull up the USB bus enable
	// Initialize the port and the pin
	GPIO_InitTypeDef   GPIO_InitStructure;
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init( GPIOD, &GPIO_InitStructure );
	USB_Control_Disable();
}

void USB_Control_Enable()
{
	HAL_GPIO_WritePin( GPIOD, GPIO_PIN_2, GPIO_PIN_RESET );
}

void USB_Control_Disable()
{
	HAL_GPIO_WritePin( GPIOD, GPIO_PIN_2, GPIO_PIN_SET );
}

void USB_Device_Connected()
{
	UserInterface_Led_On(LED2);
}

void USB_Device_Disconnected()
{
	UserInterface_Led_On(LED2);
}
