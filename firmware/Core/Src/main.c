/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usb_keyboard.h"
#include "user_interface.h"
#include "usb_ctrl.h"
#include "uart.h"
#include "keyscanner.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
const uint16_t KEY_DELAY = 100;
const uint16_t REPEAT_DELAY_INITIAL = 500;
const uint16_t REPEAT_DELAY = 30;
static uint8_t refuseInput = 0;
static uint8_t keyRepeating = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void setRefuseInput();
void clearRefuseInput();
int isRefuseInput();
void setKeyRepeating();
void clearKeyRepeating();
int isKeyRepeating();
void ledUpdateCallback(uint8_t ledState);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t keysScanned;
  uint8_t scanCodeBufferSize = 8;
  uint8_t scanCodeBuffer[scanCodeBufferSize];
  uint8_t lastScanCode = 0;
  uint8_t modifiers;
  uint8_t lastModifiers = 0;
  uint16_t repeatDelay = KEY_DELAY;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  UART_Init(&huart1);
  USB_Control_Init();
  USB_Control_Enable();

  uint8_t VERSION_STRING[] = "USB Keyboard Firmware v2.0\n";

  printf("%s", VERSION_STRING);

  KS_Init();
  UserInterface_Init();

  // toggle LEDs to show board is initialized
  UserInterface_Led_On(NUM_LOCK);
  HAL_Delay(100);
  UserInterface_Led_On(CAPS_LOCK);
  HAL_Delay(100);
  UserInterface_Led_On(SCROLL_LOCK);
  HAL_Delay(800);
  UserInterface_Led_All_Off();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  HAL_Delay(10);
	  modifiers = 0;
	  keysScanned = KS_ReadScanCode(scanCodeBuffer, scanCodeBufferSize, &modifiers);

	  // have the modifiers changed but the scan code stayed the same?
	  if ((modifiers != lastModifiers) && (lastScanCode > 0) && (lastScanCode == scanCodeBuffer[0]))
	  {
		  // stop accepting input until key(s) are released
		  setRefuseInput();
		  lastModifiers = modifiers;
	  }
	  else if(keysScanned > 0 || modifiers != 0)
	  {
		  HAL_Delay(repeatDelay);

		  if ((keysScanned > 0) && (modifiers == lastModifiers))
		  {
			  printf("Key: ");
			  for(uint8_t i = 0; i < keysScanned; i++)
			  {
				  printf("0x%02X ", scanCodeBuffer[i]);
			  }
			  printf(" (modifier 0x%02X)\n", modifiers);

			  if (!isRefuseInput())
			  {
				  USB_Send_Key_Press(scanCodeBuffer[0], modifiers);
				  // if the key is "repeating", reduce the time between repeats
				  if (scanCodeBuffer[0] == lastScanCode)
				  {
					  if (isKeyRepeating())
					  {
						  repeatDelay = REPEAT_DELAY;
					  }
					  else {
						  repeatDelay = REPEAT_DELAY_INITIAL;
						  setKeyRepeating();
					  }
				  }
				  else
				  {
					  repeatDelay = KEY_DELAY;
					  clearKeyRepeating();
				  }
				  lastScanCode = scanCodeBuffer[0];
			  }
			  else if (scanCodeBuffer[0] != lastScanCode)
			  {
				  clearRefuseInput();
				  clearKeyRepeating();
			  }
		  }
		  else
		  {
			  USB_Send_Key_Press(0x00, modifiers);
		  }

		  if (!isRefuseInput())
		  {
			  HAL_Delay(10);
			  USB_Send_All_Keys_Released(0x00);
		  }

		  lastModifiers = modifiers;
	  }
	  else
	  {
		  clearRefuseInput();
		  clearKeyRepeating();
		  repeatDelay = KEY_DELAY;
		  lastScanCode = 0;
		  lastModifiers = 0;
	  }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, CAPS_LOCK_LED_Pin|SCROLL_LOCK_LED_Pin|NUM_LOCK_LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, PA0_IO_Pin|PA1_IO_Pin|PA2_IO_Pin|PA3_IO_Pin 
                          |PA4_IO_Pin|PA5_IO_Pin|PA6_IO_Pin|PA7_IO_Pin 
                          |USB_EN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : CAPS_LOCK_LED_Pin SCROLL_LOCK_LED_Pin NUM_LOCK_LED_Pin */
  GPIO_InitStruct.Pin = CAPS_LOCK_LED_Pin|SCROLL_LOCK_LED_Pin|NUM_LOCK_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0_IO_Pin PA1_IO_Pin PA2_IO_Pin PA3_IO_Pin 
                           PA4_IO_Pin PA5_IO_Pin PA6_IO_Pin PA7_IO_Pin 
                           USB_EN_Pin */
  GPIO_InitStruct.Pin = PA0_IO_Pin|PA1_IO_Pin|PA2_IO_Pin|PA3_IO_Pin 
                          |PA4_IO_Pin|PA5_IO_Pin|PA6_IO_Pin|PA7_IO_Pin 
                          |USB_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0_INPUT_Pin PB1_INPUT_Pin PB2_INPUT_Pin PB10_INPUT_Pin 
                           PB11_INPUT_Pin PB12_INPUT_Pin PB13_INPUT_Pin PB14_INPUT_Pin 
                           PB15_INPUT_Pin PB3_INPUT_Pin PB4_INPUT_Pin PB5_INPUT_Pin 
                           PB6_INPUT_Pin PB7_INPUT_Pin PB8_INPUT_Pin PB9_INPUT_Pin */
  GPIO_InitStruct.Pin = PB0_INPUT_Pin|PB1_INPUT_Pin|PB2_INPUT_Pin|PB10_INPUT_Pin 
                          |PB11_INPUT_Pin|PB12_INPUT_Pin|PB13_INPUT_Pin|PB14_INPUT_Pin 
                          |PB15_INPUT_Pin|PB3_INPUT_Pin|PB4_INPUT_Pin|PB5_INPUT_Pin 
                          |PB6_INPUT_Pin|PB7_INPUT_Pin|PB8_INPUT_Pin|PB9_INPUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void setRefuseInput() {
	refuseInput = 1;
}

void clearRefuseInput() {
	refuseInput = 0;
}

int isRefuseInput() {
	return refuseInput == 1;
}

void setKeyRepeating() {
	keyRepeating = 1;
}

void clearKeyRepeating() {
	keyRepeating = 0;
}

int isKeyRepeating() {
	return keyRepeating == 1;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
