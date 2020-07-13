#ifndef __USB_KEYBOARD_H__
#define __USB_KEYBOARD_H__

#include "stm32f1xx.h"


void USB_Send_Key_Press(char c, uint8_t modifier);

void USB_Send_Key_Release(char c, uint8_t modifier);

void USB_Send_All_Keys_Released(uint8_t modifier);

#endif /* __USB_KEYBOARD_H__ */
