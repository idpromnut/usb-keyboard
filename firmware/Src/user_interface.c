
#include "user_interface.h"
#include "stm32f1xx_hal.h"

void UserInterface_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	// Initialize Key1
	GPIO_InitStructure.Pin = GPIO_PIN_0;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStructure );

	// Initialize Key2
	GPIO_InitStructure.Pin = GPIO_PIN_1;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStructure );

	// Initialize Led1
	GPIO_InitStructure.Pin = GPIO_PIN_2;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStructure );

	// Initialize Led2
	GPIO_InitStructure.Pin = GPIO_PIN_3;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStructure );
}

int UserInterface_Is_Button_Pressed(UserInterface_ButtonType button)
{
	if (button == KEY1)
	{
		return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET;
	}
	else if (button == KEY2)
	{
		return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET;
	}
	return 1 == 0;
}

void UserInterface_Led_On(UserInterface_LedType led)
{
	if (led == LED1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
	}
	else if (led == LED2)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
	}
}

void UserInterface_Led_Off(UserInterface_LedType led)
{
	if (led == LED1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
	}
	else if (led == LED2)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
	}
}
