/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include "usbd_hid.h"
#include "usb_device.h"
#include "devicecontrol/uart.h"
#include "devicecontrol/usb_ctrl.h"
#include "devicecontrol/usb_keyboard.h"
#include "devicecontrol/user_interface.h"
#include "keyscanner/keyscanner.h"

/* Private variables ---------------------------------------------------------*/
uint8_t refuseInput = 0;
uint8_t keyRepeating = 0;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

void setRefuseInput();
void clearRefuseInput();
int isRefuseInput();
void setKeyRepeating();
void clearKeyRepeating();
int isKeyRepeating();
void ledUpdateCallback(uint8_t ledState);

int main(void)
{
  /* MCU Configuration----------------------------------------------------------*/
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  UART_Init();
  MX_USB_DEVICE_Init();

  // setup the Led callback
  USBD_HID_SetLedStateCallback(ledUpdateCallback);

  USB_Control_Init();
  USB_Control_Enable();

  uint8_t VERSION_STRING[] = "USB Keyboard Firmware v0.1\n";

  const uint16_t KEY_DELAY = 100;
  const uint16_t REPEAT_DELAY_INITIAL = 500;
  const uint16_t REPEAT_DELAY = 30;

  printf("%s", VERSION_STRING);

  uint8_t keysScanned;
  uint8_t scanCodeBufferSize = 8;
  uint8_t scanCodeBuffer[scanCodeBufferSize];
  uint8_t lastScanCode = 0;
  uint8_t modifiers;
  uint8_t lastModifiers = 0;
  uint16_t repeatDelay = KEY_DELAY;

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

  while (1)
  {
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
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

//  HAL_RCC_MCOConfig(RCC_MCO, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_1);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
//  __HAL_RCC_GPIOD_CLK_ENABLE();
}

void ledUpdateCallback(uint8_t ledState)
{
	UserInterface_Led_Set(NUM_LOCK, ledState & 0x01);
	UserInterface_Led_Set(CAPS_LOCK, (ledState >> 1) & 0x01);
	UserInterface_Led_Set(SCROLL_LOCK, (ledState >> 2) & 0x01);
}

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

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif
