#include "main.h"
#include "keyscanner.h"
#include "stm32f1xx_hal.h"
#include "keymaps/ibm-model-m-101.h"

// private functions
uint16_t KS_ScanForKeyPress(uint8_t iActiveLine);
uint8_t KS_IsModifier(uint16_t scanCode);
void KS_UpdateModifiers(uint16_t scanCode, uint8_t* modifiers);
void KS_GpioPinInit(GPIO_TypeDef *iPort, uint32_t iPin, uint32_t iMode, uint32_t iPull, uint32_t iSpeed);

void KS_Init()
{
	// remap JTAG to GPIO, leaving SWD enabled
	__HAL_AFIO_REMAP_SWJ_NOJTAG();

	// init input scan lines
	KS_GpioPinInit(PB0_INPUT_GPIO_Port, PB0_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);  //  1
	KS_GpioPinInit(PB1_INPUT_GPIO_Port, PB1_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);  //  2
	KS_GpioPinInit(PB2_INPUT_GPIO_Port, PB2_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);  //  3
	KS_GpioPinInit(PB3_INPUT_GPIO_Port, PB3_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);  //  4
	KS_GpioPinInit(PB4_INPUT_GPIO_Port, PB4_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);  //  5
	KS_GpioPinInit(PB5_INPUT_GPIO_Port, PB5_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);  //  6
	KS_GpioPinInit(PB6_INPUT_GPIO_Port, PB6_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);  //  6
	KS_GpioPinInit(PB7_INPUT_GPIO_Port, PB7_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);  //  8
	KS_GpioPinInit(PB8_INPUT_GPIO_Port, PB8_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);  //  9
	KS_GpioPinInit(PB9_INPUT_GPIO_Port, PB9_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW);  // 10
	KS_GpioPinInit(PB10_INPUT_GPIO_Port, PB10_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW); // 11
	KS_GpioPinInit(PB11_INPUT_GPIO_Port, PB11_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW); // 12
	KS_GpioPinInit(PB12_INPUT_GPIO_Port, PB12_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW); // 13
	KS_GpioPinInit(PB13_INPUT_GPIO_Port, PB13_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW); // 14
	KS_GpioPinInit(PB14_INPUT_GPIO_Port, PB14_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW); // 15
	KS_GpioPinInit(PB15_INPUT_GPIO_Port, PB15_INPUT_Pin, GPIO_MODE_INPUT, GPIO_PULLDOWN, GPIO_SPEED_FREQ_LOW); // 16

	// init output scan lines
	KS_GpioPinInit(PA0_IO_GPIO_Port, PA0_IO_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(PA1_IO_GPIO_Port, PA1_IO_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(PA2_IO_GPIO_Port, PA2_IO_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(PA3_IO_GPIO_Port, PA3_IO_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(PA4_IO_GPIO_Port, PA4_IO_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(PA5_IO_GPIO_Port, PA5_IO_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(PA6_IO_GPIO_Port, PA6_IO_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
	KS_GpioPinInit(PA7_IO_GPIO_Port, PA7_IO_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW);
}


uint16_t KS_ScanForKeyPress(uint8_t iActiveLine)
{
	uint32_t outputRegister = (GPIOA->ODR & 0xFFFFFF00) | (uint32_t)iActiveLine;
	GPIOA->ODR = outputRegister;
	uint16_t scannedInput = (uint16_t)GPIOB->IDR;
	return scannedInput;
}

#ifdef DEBUG
void KS_PrintScanLines()
{
	uint16_t scannedInput;
	uint8_t keyScans = 0;

	for(uint8_t i = 0; i < 8; ++i)
	{
		scannedInput = KS_ScanForKeyPress(1 << (7 - i));
		if (scannedInput > 0)
		{
			// detected a key press
			printf("Key press detected at %d/0x%04X\n", (i+1), scannedInput);
			++keyScans;
		}
	}
}
#else
void KS_PrintScanLines() {}
#endif

uint8_t KS_ReadScanCode(uint8_t* scanCodeBuffer, uint8_t bufferLength, uint8_t* modifiers)
{
	uint16_t scannedInput;
	uint8_t keyStrokes = 0;
	uint16_t scanCode;

	for(uint8_t i = 0; i < 8; ++i)
	{
		scannedInput = KS_ScanForKeyPress(1 << (7 - i));
		for(uint8_t j = 0; j < 16; j++)
		{
			// decode detected a key press(es)
			if (((scannedInput >> j) & 0x0001) == 1)
			{
				scanCode = KEY_MAP[i][0x0F - j];
				if (scanCode > 0)
				{
					if (KS_IsModifier(scanCode))
					{
						KS_UpdateModifiers(scanCode, modifiers);
					}
					else if (keyStrokes < bufferLength)
					{
						keyStrokes++;
						scanCodeBuffer[keyStrokes - 1] = scanCode;
					}
				}
				else
				{
					#ifdef DEBUG
					printf("No key-code found for scanned key: %i/%i (scanned input line was 0x%04X\n", i, (0x0F - j), scannedInput);
					#endif
				}
			}
		}
	}

	return keyStrokes;
}

uint8_t KS_IsModifier(uint16_t scanCode)
{
	switch(scanCode)
	{
		case 0xE0:
		case 0xE1:
		case 0xE2:
		case 0xE3:
		case 0xE4:
		case 0xE5:
		case 0xE6:
		case 0xE7:
			return 1==1;
		default:
			return 1==0;
	}
}

void KS_UpdateModifiers(uint16_t scanCode, uint8_t* modifiers)
{
	*modifiers |= 1 << (uint8_t)(0x000F & scanCode);
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
