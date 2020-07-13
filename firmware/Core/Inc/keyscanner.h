#ifndef __KEY_SCANNER_H__
#define __KEY_SCANNER_H__

#include "stm32f1xx_hal.h"


void KS_Init();
void KS_PrintScanLines();
uint8_t KS_ReadScanCode(uint8_t* scanCodeBuffer, uint8_t bufferLength, uint8_t* modifiers);

#endif /* __KEY_SCANNER_H__ */
