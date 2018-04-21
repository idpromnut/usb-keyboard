#ifndef __USB_CTRL_H__
#define __USB_CTRL_H__

#define USB_ENABLE_PORT		GPIOA
#define USB_ENABLE_PIN		GPIO_PIN_8

void USB_Control_Init();
void USB_Control_Enable();
void USB_Control_Disable();
void USB_Device_Connected();
void USB_Device_Disconnected();

#endif /* __USB_CTRL_H__ */
