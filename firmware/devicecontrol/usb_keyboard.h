
#ifndef _USB_KEYBOARD_H
#define _USB_KEYBOARD_H

#include "stm32f1xx.h"


void USB_Send_Key_Press(char c, uint8_t modifier);

void USB_Send_Key_Release(char c);

void USB_Send_All_Keys_Released(void);

#endif
