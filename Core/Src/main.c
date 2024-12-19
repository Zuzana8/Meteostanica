/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "spi.h"
#include "gpio.h"
#include "ili9163.h"
#include "rtc.h"
#include <time.h>
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void TempHumPres(float temperature, float humidity, int pressure);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t rx_data = 0;
uint8_t switchState = 0; // 0 = main program, 1 = graph
float temperatureHistory[24 * 7] = {0}; // Array for 7 days of hourly temperatures
uint8_t graphDrawn = 0;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

float getTemperature(void){

	return (rand() % 21) + 10;
}

float getHumidity(void){

	return (rand() % 70) + 10;
}

int getPressure(void){
	return (rand() % 75) + 980;
}

int main(void)
{
	srand((unsigned int)time(NULL));
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */


  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();

  initCD_Pin();
  initCS_Pin();
  initRES_Pin();

  LL_mDelay(50);

  lcdInitialise(0x48);
  lcdClearDisplay(0xFFFF);

  RTC_Init();
//  lcdPutS("VRS 2019", lcdTextX(2), lcdTextY(1), 0x0000, 0xFFFF);
//  lcdPutS("Cvicenie 8", lcdTextX(2), lcdTextY(4), 0x0000, 0xFFFF);
//  lcdPutS("SPI komunikacia", lcdTextX(2), lcdTextY(5), 0x0000, 0xFFFF);


  for (int i = 0; i < 8 * 7; i++) {
      temperatureHistory[i] = (rand() % 21) + 10; // Random temperatures between 10 and 30 degrees
  }

  	uint8_t lastWeather = 0;
	while (1)
	{
		if (LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_3)) {
		    printf("Button pressed!\n");
		}
		//switchState = LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_1); // Replace with your switch GPIO pin

		if (switchState){
			if (!graphDrawn) {
		    // Draw the graph
		    drawTemperatureGraph(temperatureHistory, 8 * 7);
		    LL_mDelay(1000); // Avoid rapid refresh of the graph
		    graphDrawn = 1;
			}
		} else {


			if (graphDrawn == 1){
				lcdClearDisplay(decodeRgbValue(0xFF, 0xFF, 0xFF));
				graphDrawn = 0;
			}


			float temperature = getTemperature();
			float humidity = getTemperature();
			float pressure = getPressure();

			uint8_t currentWeather;

			// Determine current weather based on pressure and humidity
			if (pressure > 1013.25 && humidity < 60.0) //1013.25 je priemerny tlak vo vyske 1500m
			{
			// High pressure and low humidity indicate sunny weather
			currentWeather = SUNNY_SYMBOL_INDEX;
			}
			else if (pressure < 1000.0 && humidity > 70.0)
			{
			// Low pressure and high humidity indicate rainy weather
			currentWeather = RAINY_SYMBOL_INDEX;
			}
			else
			{
			// Otherwise, assume cloudy weather
			currentWeather = CLOUDY_SYMBOL_INDEX;
		}


			if (currentWeather != lastWeather)
				{
					drawWeather(currentWeather);
					lastWeather = currentWeather; // Update the last displayed weather
				}

			get_current_time();

			display_time_with_large_digits();

			TempHumPres(temperature, humidity, pressure);

			LL_mDelay(500);
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
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0)
  {
  Error_Handler();
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1)
  {

  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {

  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
}

void TempHumPres(float temperature, float humidity, int pressure)
{
    char buffer[10];

    // Display the static labels for temperature and humidity
    lcdPutLargeS("Tem.:", 10, 90, 0x0000, 0xFFFF);       // Black text on white background
    lcdPutLargeS("Hum.:", 170, 90, 0x0000, 0xFFFF);    // Black text on white background
    lcdPutLargeS("Pres.:", 10, 75, 0x0000, 0xFFFF);    // Black text on white background

    //dlhodobá predpoveď
    lcdPutLargeS("Fairly Fine Showery     Later", 10, 140, 0x0000, 0xFFFF);

    // Display the temperature value
    snprintf(buffer, sizeof(buffer), "%.1f C", temperature);
    lcdPutLargeS(buffer, 70, 90, 0x0000, 0xFFFF);   // Display temperature in large font

    // Display the humidity value
    snprintf(buffer, sizeof(buffer), "%.1f %%", humidity);
    lcdPutLargeS(buffer, 230, 90, 0x0000, 0xFFFF);   // Display humidity in large font

    // Display the humidity value
    snprintf(buffer, sizeof(buffer), "%d hPa", pressure);
    lcdPutLargeS(buffer, 80, 75, 0x0000, 0xFFFF);   // Display humidity in large font
}

/* USER CODE BEGIN 4 */

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

/* USER CODE BEGIN 0 */
void EXTI3_IRQHandler(void)
{

        if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3))
        {
            LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3); // Clear the interrupt flag
            switchState = !switchState; // Toggle switch state
        }

}

/* USER CODE END 0 */


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
