#ifndef __KEY_SCANNER_H
#define __KEY_SCANNER_H

#include "stm32f1xx_hal.h"


void KS_Init();
uint16_t KS_ScanForKeyPress(uint8_t iActiveLine);
void KS_FormatScanLines(uint8_t iActiveLines, uint16_t iReadLines);
uint8_t KS_PrintScanLines();
uint8_t KS_ReadScanCode();
void KS_GpioPinInit(GPIO_TypeDef *iPort, uint32_t iPin, uint32_t iMode, uint32_t iPull, uint32_t iSpeed);

#endif
