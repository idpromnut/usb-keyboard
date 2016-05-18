
#ifndef _USER_INTERFACE_H
#define _USER_INTERFACE_H

typedef enum {
	KEY1,
	KEY2
} UserInterface_ButtonType;

typedef enum {
	LED1,
	LED2
} UserInterface_LedType;

void UserInterface_Init(void);

int UserInterface_Is_Button_Pressed(UserInterface_ButtonType button);

void UserInterface_Led_On(UserInterface_LedType led);

void UserInterface_Led_Off(UserInterface_LedType led);

#endif // _USER_INTERFACE_H
