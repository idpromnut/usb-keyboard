
#include "keyscanner.h"
#include "stm32f1xx_hal.h"

void KS_Init()
{
	// init input scan lines
	KS_GpioPinInit(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOA, GPIO_PIN_1, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOA, GPIO_PIN_2, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOA, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOA, GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOA, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOA, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOA, GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOC, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOC, GPIO_PIN_1, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOC, GPIO_PIN_2, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOC, GPIO_PIN_3, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOC, GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOC, GPIO_PIN_5, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOC, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOC, GPIO_PIN_7, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);

	// init output scan lines
	KS_GpioPinInit(GPIOB, GPIO_PIN_8, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOB, GPIO_PIN_9, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOB, GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOB, GPIO_PIN_11, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOB, GPIO_PIN_12, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOB, GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOB, GPIO_PIN_14, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
}


uint16_t KS_ScanForKeyPress(uint8_t iActiveLine)
{
	uint32_t outputRegister = ((uint32_t)iActiveLine << 8);
	GPIOB->ODR = outputRegister;
	uint32_t scannedInput = (GPIOA->IDR & 0x000000FF) << 8;
	scannedInput |= (GPIOC->IDR & 0x000000FF);
	return scannedInput;
}

void KS_FormatScanLines(uint8_t iActiveLine, uint16_t iReadLines)
{
}

uint8_t KS_PrintScanLines()
{
	uint16_t scannedInput;
	uint8_t keyScans = 0;

	for(uint8_t i = 0; i < 8; ++i)
	{
		scannedInput = KS_ScanForKeyPress(1 << i);
		if (scannedInput > 0)
		{
			// detected a key press
			printf("Key press detected at %d/0x%04X\n", (i+1), scannedInput);
			++keyScans;
		}
	}

	return keyScans;
}

void KS_GpioPinInit(GPIO_TypeDef *iPort, uint32_t iPin, uint32_t iMode, uint32_t iPull, uint32_t iSpeed)
{
	// Initialize the port and the pin
	GPIO_InitTypeDef   GPIO_InitStructure;
	GPIO_InitStructure.Pin = iPin;
	GPIO_InitStructure.Mode = iMode;
	GPIO_InitStructure.Pull = iPull;
	GPIO_InitStructure.Speed = iSpeed;
	HAL_GPIO_Init( iPort, &GPIO_InitStructure );
}
