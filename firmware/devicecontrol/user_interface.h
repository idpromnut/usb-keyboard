#ifndef _USER_INTERFACE_H
#define _USER_INTERFACE_H

#include "stm32f1xx.h"

typedef enum {
	CAPS_LOCK,
	SCROLL_LOCK,
	NUM_LOCK
} UserInterface_LedType;


#define LED_PORT 			GPIOC
#define CAPS_LOCK_PIN		GPIO_PIN_15
#define SCROLL_LOCK_PIN		GPIO_PIN_14
#define NUM_LOCK_PIN		GPIO_PIN_13


void UserInterface_Init(void);

void UserInterface_Led_On(UserInterface_LedType led);

void UserInterface_Led_Off(UserInterface_LedType led);

void UserInterface_Led_All_Off();

void UserInterface_Led_Set(UserInterface_LedType led, uint8_t state);

#endif // _USER_INTERFACE_H
