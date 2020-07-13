/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CAPS_LOCK_LED_Pin GPIO_PIN_13
#define CAPS_LOCK_LED_GPIO_Port GPIOC
#define SCROLL_LOCK_LED_Pin GPIO_PIN_14
#define SCROLL_LOCK_LED_GPIO_Port GPIOC
#define NUM_LOCK_LED_Pin GPIO_PIN_15
#define NUM_LOCK_LED_GPIO_Port GPIOC
#define PA0_IO_Pin GPIO_PIN_0
#define PA0_IO_GPIO_Port GPIOA
#define PA1_IO_Pin GPIO_PIN_1
#define PA1_IO_GPIO_Port GPIOA
#define PA2_IO_Pin GPIO_PIN_2
#define PA2_IO_GPIO_Port GPIOA
#define PA3_IO_Pin GPIO_PIN_3
#define PA3_IO_GPIO_Port GPIOA
#define PA4_IO_Pin GPIO_PIN_4
#define PA4_IO_GPIO_Port GPIOA
#define PA5_IO_Pin GPIO_PIN_5
#define PA5_IO_GPIO_Port GPIOA
#define PA6_IO_Pin GPIO_PIN_6
#define PA6_IO_GPIO_Port GPIOA
#define PA7_IO_Pin GPIO_PIN_7
#define PA7_IO_GPIO_Port GPIOA
#define PB0_INPUT_Pin GPIO_PIN_0
#define PB0_INPUT_GPIO_Port GPIOB
#define PB1_INPUT_Pin GPIO_PIN_1
#define PB1_INPUT_GPIO_Port GPIOB
#define PB2_INPUT_Pin GPIO_PIN_2
#define PB2_INPUT_GPIO_Port GPIOB
#define PB10_INPUT_Pin GPIO_PIN_10
#define PB10_INPUT_GPIO_Port GPIOB
#define PB11_INPUT_Pin GPIO_PIN_11
#define PB11_INPUT_GPIO_Port GPIOB
#define PB12_INPUT_Pin GPIO_PIN_12
#define PB12_INPUT_GPIO_Port GPIOB
#define PB13_INPUT_Pin GPIO_PIN_13
#define PB13_INPUT_GPIO_Port GPIOB
#define PB14_INPUT_Pin GPIO_PIN_14
#define PB14_INPUT_GPIO_Port GPIOB
#define PB15_INPUT_Pin GPIO_PIN_15
#define PB15_INPUT_GPIO_Port GPIOB
#define USB_EN_Pin GPIO_PIN_8
#define USB_EN_GPIO_Port GPIOA
#define PB3_INPUT_Pin GPIO_PIN_3
#define PB3_INPUT_GPIO_Port GPIOB
#define PB4_INPUT_Pin GPIO_PIN_4
#define PB4_INPUT_GPIO_Port GPIOB
#define PB5_INPUT_Pin GPIO_PIN_5
#define PB5_INPUT_GPIO_Port GPIOB
#define PB6_INPUT_Pin GPIO_PIN_6
#define PB6_INPUT_GPIO_Port GPIOB
#define PB7_INPUT_Pin GPIO_PIN_7
#define PB7_INPUT_GPIO_Port GPIOB
#define PB8_INPUT_Pin GPIO_PIN_8
#define PB8_INPUT_GPIO_Port GPIOB
#define PB9_INPUT_Pin GPIO_PIN_9
#define PB9_INPUT_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
