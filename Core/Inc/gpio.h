/*
 * gpio.h
 *
 *  Created on: Nov 26, 2024
 *      Author: prezentacie
 */

//#ifndef GPIO_H
//#define GPIO_H
//
//#include "stm32f3xx_hal.h"
//
//// Define GPIO Ports and Pins for ILI9341
//#define DCX_GPIO_Port GPIOB
//#define DCX_Pin       GPIO_PIN_0   // Choose the appropriate pin
//#define CSX_GPIO_Port GPIOA
//#define CSX_Pin       GPIO_PIN_4   // Choose the appropriate pin
//#define RESX_GPIO_Port GPIOB
//#define RESX_Pin      GPIO_PIN_2   // Choose the appropriate pin
//
//// Function to initialize GPIO for ILI9341
//void MX_GPIO_Init(void);
//
//#endif // GPIO_H

#ifndef GPIO_H
#define GPIO_H

#include "stm32f3xx_hal.h"

// Define GPIO Ports and Pins for ILI9341
#define DCX_GPIO_Port GPIOB
#define DCX_Pin       GPIO_PIN_3   // PB3 pre DC (D3 podľa schémy)

#define CSX_GPIO_Port GPIOA
#define CSX_Pin       GPIO_PIN_4   // PA4 pre CS (A3 podľa schémy)

#define RESX_GPIO_Port GPIOB
#define RESX_Pin      GPIO_PIN_6   // PB6 pre RESET (D6 podľa schémy)

#define SCK_GPIO_Port GPIOA
#define SCK_Pin       GPIO_PIN_5   // PA5 pre SCK (A4 podľa schémy)

#define SDI_GPIO_Port GPIOA
#define SDI_Pin       GPIO_PIN_7   // PA7 pre MOSI (A6 podľa schémy)

#define SDO_GPIO_Port GPIOA
#define SDO_Pin       GPIO_PIN_6   // PA6 pre MISO (A5 podľa schémy)

// Function to initialize GPIO for ILI9341
void MX_GPIO_Init(void);

#endif // GPIO_H

