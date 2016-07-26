
#ifndef _USER_INTERFACE_H
#define _USER_INTERFACE_H

typedef enum {
	LED1,
	LED2,
	LED3
} UserInterface_LedType;

void UserInterface_Init(void);

void UserInterface_Led_On(UserInterface_LedType led);

void UserInterface_Led_Off(UserInterface_LedType led);

#endif // _USER_INTERFACE_H
