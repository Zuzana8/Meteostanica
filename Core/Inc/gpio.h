/*
 * gpio.h
 *
 *  Created on: Nov 26, 2024
 *      Author: prezentacie
 */

#ifndef GPIO_H
#define GPIO_H

#include "stm32f3xx_hal.h"

// Define GPIO Ports and Pins for ILI9341
#define DCX_GPIO_Port GPIOB
#define DCX_Pin       GPIO_PIN_0   // Choose the appropriate pin
#define CSX_GPIO_Port GPIOB
#define CSX_Pin       GPIO_PIN_1   // Choose the appropriate pin
#define RESX_GPIO_Port GPIOB
#define RESX_Pin      GPIO_PIN_2   // Choose the appropriate pin

// Function to initialize GPIO for ILI9341
void MX_GPIO_Init(void);

#endif // GPIO_H
