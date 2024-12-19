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
#include "fatfs.h"
#include "spi.h"
#include "gpio.h"
#include "ili9163.h"
#include "rtc.h"
#include <time.h>
#include <stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include <stdbool.h>
#include "lps25hb.h"
#include "hts221.h"
#include "i2c.h"
#include "zambretti.h"
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
RTC_HandleTypeDef hrtc;
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart2;
/* USER CODE BEGIN PV */
const uint16_t max_input_size=2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
//static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_RTC_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
void TempHumPres(float temperature, float humidity, int pressure);
void UART_Receive(uint8_t *data, uint16_t size);
void myprintf(const char *fmt, ...);
void Set_RTC_Time(void);
void Set_RTC_Date(void);
void Set_RTC_Alarm(void);
void process_SD_card();
void printTime();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
float temperature = 0;
float humidity = 0;
float pressure = 0;
char z_text[50];
uint8_t rx_data = 0;
uint8_t switchState = 0; // 0 = main program, 1 = graph
float temperatureHistory[24 * 7] = {0}; // Array for 7 days of hourly temperatures
uint8_t graphDrawn = 0;

float getTemperature(void){

	return (rand() % 21) + 10;
}

float getHumidity(void){

	return (rand() % 70) + 10;
}

int getPressure(void){
	return (rand() % 75) + 980;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */



int main(void)
{
  /* USER CODE BEGIN 1 */
	srand((unsigned int)time(NULL));
  /* USER CODE END 1 */


  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
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
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_FATFS_Init();
  MX_RTC_Init();
  MX_USART2_UART_Init();
  MX_TIM2_Init();
  RTC_Init();

  //display inits
  initCD_Pin();
  initCS_Pin();
  initRES_Pin();
  LL_mDelay(50);
  lcdInitialise(0x48);
  lcdClearDisplay(0xFFFF);

   //initialize sensors
  HTS221_init();
  LPS25HB_init();

  //init time
  Set_RTC_Time();
  Set_RTC_Date();
  //HAL_TIM_Base_Start(&htim2);
  HAL_TIM_Base_Start_IT(&htim2);
  //HAL_RTC_EnableAlarm(&hrtc, RTC_ALARM_TYPE_ALARM_A);
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	char buffer[50];
	HAL_RTC_GetTime(&hrtc, &sTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
//	sprintf(buffer,"Current Time: %02d:%02d:%02d and date is: %02d-%02d-20%02d \n\r",sTime.Hours, sTime.Minutes, sTime.Seconds, sDate.Date, sDate.Month, sDate.Year);
//    HAL_UART_Transmit(&huart2, buffer, sizeof(buffer), HAL_MAX_DELAY);


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
      float temp = hts221_get_temperature();
	    float hum = hts221_get_humidity();
	    float press = lps25hb_get_pressure();
	    zambretti(press, temp, z_text);
      
	    //calculate prediction
	    char text[50];
	    strcpy(text, z_text);

	  LL_mDelay(1000);
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

//  THISIS FROM SENSORS AND TIMERS
//   void SystemClock_Config(void)
// {
//   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
//   RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

//   /** Initializes the RCC Oscillators according to the specified parameters
//   * in the RCC_OscInitTypeDef structure.
//   */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
//   RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//   RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//   RCC_OscInitStruct.LSIState = RCC_LSI_ON;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
//   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//   {
//     Error_Handler();
//   }

//   /** Initializes the CPU, AHB and APB buses clocks
//   */
//   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                               |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
//   PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
//   PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
//   if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
//   {
//     Error_Handler();
//   }
// }
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

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}


/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 2399999999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel3_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, DISPLAY_CS_Pin|SD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Button___ext__interrupt_Pin */
  GPIO_InitStruct.Pin = Button___ext__interrupt_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Button___ext__interrupt_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : DISPLAY_CS_Pin SD_CS_Pin */
  GPIO_InitStruct.Pin = DISPLAY_CS_Pin|SD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//TIMER FUNCTIONS

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim2 )
  {
	  printTime();
  }
}
void printTime(){
	RTC_TimeTypeDef sTime;
	RTC_DateTypeDef sDate;
	char buffer[50];
	HAL_RTC_GetTime(&hrtc, &sTime,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	sprintf(buffer,"Current Time: %02d:%02d:%02d and date is: %02d-%02d-20%02d \n\r\t",sTime.Hours, sTime.Minutes, sTime.Seconds, sDate.Date, sDate.Month, sDate.Year);
	HAL_UART_Transmit(&huart2, buffer, sizeof(buffer), HAL_MAX_DELAY);

}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc) {

	char buffer[50];
	sprintf(buffer,"I AM THE ALARM YOUARE LOOKING FOR");
    HAL_UART_Transmit(&huart2, buffer, sizeof(buffer), HAL_MAX_DELAY);

}


void Set_RTC_Time(void)
{

    RTC_TimeTypeDef sTime = {0};
    uint8_t buffer[3];
    uint8_t i = 0;

    HAL_UART_Transmit(&huart2, (uint8_t*)"Enter Hours (00-23): ", 21, HAL_MAX_DELAY);
    UART_Receive(buffer, sizeof(buffer));

    while (buffer[i] == '\n' || buffer[i] == '\r') {
        i++;
    }

    sTime.Hours = (buffer[i] - '0') * 10 + (buffer[i+1] - '0');

    i=0;
    HAL_UART_Transmit(&huart2,
    		(uint8_t*)"\n\rEnter Minutes (00-59): ", 24, HAL_MAX_DELAY);
    UART_Receive(buffer, sizeof(buffer));
    sTime.Minutes = (buffer[i] - '0') * 10 + (buffer[i+1] - '0');

    i=0;
    HAL_UART_Transmit(&huart2, (uint8_t*)"\n\rEnter Seconds (00-59): \n\r", 24, HAL_MAX_DELAY);
    UART_Receive(buffer, sizeof(buffer));
    sTime.Seconds = (buffer[i] - '0') * 10 + (buffer[i + 1] - '0');

    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;

    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_UART_Transmit(&huart2, (uint8_t*)"\n\rTime is set!\n\r ", 16, HAL_MAX_DELAY);
}

void Set_RTC_Date(void)
{
    RTC_DateTypeDef sDate = {0};
    uint8_t buffer[3];
    int i=0;

    HAL_UART_Transmit(&huart2, (uint8_t*)"Enter Year (00-99): ", 20, HAL_MAX_DELAY);
    UART_Receive(buffer, sizeof(buffer));
    sDate.Year = (buffer[0] - '0') * 10 + (buffer[1] - '0');

    HAL_UART_Transmit(&huart2, (uint8_t*)"\n\rEnter Month (01-12): ", 22, HAL_MAX_DELAY);
    UART_Receive(buffer, sizeof(buffer));
    sDate.Month = (buffer[0] - '0') * 10 + (buffer[1] - '0');

    HAL_UART_Transmit(&huart2, (uint8_t*)"\n\rEnter Date (01-31): ", 21, HAL_MAX_DELAY);
    UART_Receive(buffer, sizeof(buffer));
    sDate.Date = (buffer[0] - '0') * 10 + (buffer[1] - '0');

    while (buffer[i] == '\n' || buffer[i] == '\r') {
        i++;
    }
    HAL_UART_Transmit(&huart2, (uint8_t*)"\n\rEnter Weekday (1=Mon, 7=Sun): ", 33, HAL_MAX_DELAY);
    UART_Receive(buffer, 1);

    sDate.WeekDay = buffer[i] - '0';

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
    {
        Error_Handler();
    }
    HAL_UART_Transmit(&huart2, (uint8_t*)"\n\rDate is set!\n\r ", 16, HAL_MAX_DELAY);

}

void UART_Receive(uint8_t *buffer, uint16_t size)
{
    // Ensure the buffer is null-terminated
	//uint16_t size = max_input_size+1;
    HAL_UART_Receive(&huart2, buffer, size, HAL_MAX_DELAY);
    buffer[size] = '\0';
}


//SD CARD FUNCTIONS
void process_SD_card( void )
{
  FATFS       FatFs;                //Fatfs handle
  FIL         fil;                  //File handle
  FRESULT     fres;                 //Result after operations
  char        buf[100];

  do
  {
    //Mount the SD Card
    fres = f_mount(&FatFs, "", 1);    //1=mount now
    if (fres != FR_OK)
    {
      printf("No SD Card found : (%i)\r\n", fres);
      break;
    }
    printf("SD Card Mounted Successfully!!!\r\n");

    //Read the SD Card Total size and Free Size
    FATFS *pfs;
    DWORD fre_clust;
    uint32_t totalSpace, freeSpace;

    f_getfree("", &fre_clust, &pfs);
    totalSpace = (uint32_t)((pfs->n_fatent - 2) * pfs->csize * 0.5);
    freeSpace = (uint32_t)(fre_clust * pfs->csize * 0.5);

    printf("TotalSpace : %lu bytes, FreeSpace = %lu bytes\n", totalSpace, freeSpace);

    //Open the file
    fres = f_open(&fil, "EmbeTronicX.txt", FA_WRITE | FA_READ | FA_CREATE_ALWAYS);
    if(fres != FR_OK)
    {
      printf("File creation/open Error : (%i)\r\n", fres);
      break;
    }

    printf("Writing data!!!\r\n");
    //write the data
    f_puts("Welcome to EmbeTronicX", &fil);

    //close your file
    f_close(&fil);

    //Open the file
    fres = f_open(&fil, "EmbeTronicX.txt", FA_READ);
    if(fres != FR_OK)
    {
      printf("File opening Error : (%i)\r\n", fres);
      break;
    }

    //read the data
    f_gets(buf, sizeof(buf), &fil);

    printf("Read Data : %s\n", buf);

    //close your file
    f_close(&fil);
    printf("Closing File!!!\r\n");

#if 0
    //Delete the file.
    fres = f_unlink(EmbeTronicX.txt);
    if (fres != FR_OK)
    {
      printf("Cannot able to delete the file\n");
    }
#endif
  } while( false );

  //We're done, so de-mount the drive
  f_mount(NULL, "", 0);
  printf("SD Card Unmounted Successfully!!!\r\n");
}


void myprintf(const char *fmt, ...) {
	static char buffer[256];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, args);
	va_end(args);

	int len = strlen(buffer);
	HAL_UART_Transmit(&huart2, (uint8_t*) buffer, len, -1);
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

/* USER CODE BEGIN 0 */
void EXTI3_IRQHandler(void)
{

        if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3))
        {
            LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3); // Clear the interrupt flag
            switchState = !switchState; // Toggle switch state
        }

}

static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

/* USER CODE END 0 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
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
void assert_failed(char *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
