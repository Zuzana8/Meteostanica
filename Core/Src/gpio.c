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

    // Enable GPIO clocks
    __HAL_RCC_GPIOB_CLK_ENABLE();  // Adjust this for the correct GPIO port

    // Configure DCX Pin
    GPIO_InitStruct.Pin = DCX_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  // Push-pull mode
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(DCX_GPIO_Port, &GPIO_InitStruct);

    // Configure CSX Pin
    GPIO_InitStruct.Pin = CSX_Pin;
    HAL_GPIO_Init(CSX_GPIO_Port, &GPIO_InitStruct);

    // Configure RESX Pin
    GPIO_InitStruct.Pin = RESX_Pin;
    HAL_GPIO_Init(RESX_GPIO_Port, &GPIO_InitStruct);
}
