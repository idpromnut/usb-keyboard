
#include "user_interface.h"
#include "stm32f1xx_hal.h"

void UserInterface_Init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;

	// Initialize Led1, Led2, Led3
	GPIO_InitStructure.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init( GPIOA, &GPIO_InitStructure );
}


void UserInterface_Led_On(UserInterface_LedType led)
{
	if (led == LED1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else if (led == LED2)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_SET);
	}
	else if (led == LED3)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
	}
}

void UserInterface_Led_Off(UserInterface_LedType led)
{
	if (led == LED1)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);
	}
	else if (led == LED2)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_RESET);
	}
	else if (led == LED3)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
	}
}
