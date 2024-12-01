/*
 * gpio.c
 *
 *  Created on: Nov 26, 2024
 *      Author: prezentacie
 */

#include "gpio.h"

void MX_GPIO_Init(void)
{
    // GPIO Initialization Structure
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Enable GPIO clocks for the necessary ports
    __HAL_RCC_GPIOA_CLK_ENABLE();  // For SPI pins (SCK, MOSI, MISO)
    __HAL_RCC_GPIOB_CLK_ENABLE();  // For control pins (CSX, DCX, RESX)

    // Configure SPI SCK Pin (PA5)
    GPIO_InitStruct.Pin = GPIO_PIN_5;  // SCK
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  // Alternate function, push-pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;  // SPI1 alternate function
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure SPI MOSI Pin (PA7)
    GPIO_InitStruct.Pin = GPIO_PIN_7;  // MOSI
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure SPI MISO Pin (PA6) (if reading from the display is required)
    GPIO_InitStruct.Pin = GPIO_PIN_6;  // MISO
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;  // Alternate function, push-pull
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure CSX Pin (Chip Select)
    GPIO_InitStruct.Pin = CSX_Pin;  // Example: GPIO_PIN_4
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull mode
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CSX_GPIO_Port, &GPIO_InitStruct);

    // Configure DCX Pin (Data/Command Select)
    GPIO_InitStruct.Pin = DCX_Pin;  // Example: GPIO_PIN_0
    HAL_GPIO_Init(DCX_GPIO_Port, &GPIO_InitStruct);

    // Configure RESX Pin (Reset)
    GPIO_InitStruct.Pin = RESX_Pin;  // Example: GPIO_PIN_1
    HAL_GPIO_Init(RESX_GPIO_Port, &GPIO_InitStruct);

    // Set initial pin states (if needed)
    HAL_GPIO_WritePin(CSX_GPIO_Port, CSX_Pin, GPIO_PIN_SET);  // CSX high (inactive)
    HAL_GPIO_WritePin(DCX_GPIO_Port, DCX_Pin, GPIO_PIN_SET);  // DCX high
    HAL_GPIO_WritePin(RESX_GPIO_Port, RESX_Pin, GPIO_PIN_SET);  // RESX high (inactive)
}
