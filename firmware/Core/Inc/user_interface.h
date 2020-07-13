#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "stm32f1xx.h"

typedef enum {
	CAPS_LOCK,
	SCROLL_LOCK,
	NUM_LOCK
} UserInterface_LedType;


void UserInterface_Init(void);

void UserInterface_Led_On(UserInterface_LedType led);

void UserInterface_Led_Off(UserInterface_LedType led);

void UserInterface_Led_All_Off();

void UserInterface_Led_Set(UserInterface_LedType led, uint8_t state);

#endif /* __USER_INTERFACE_H__ */
