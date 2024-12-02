/*
 * ILI9341.c
 *
 *  Created on: Nov 26, 2024
 *      Author: prezentacie
 */

#include "stm32f3xx_hal.h"
#include "ILI9341.h"
#include "spi.h"
#include "gpio.h"
#include "fonts.h" // Definícia fontu (bitmapové údaje znakov)


#define ILI9341_WIDTH  240
#define ILI9341_HEIGHT 320

// Príkazy ILI9341
#define ILI9341_CMD_MEMORY_WRITE 0x2C
#define ILI9341_CMD_COLUMN_ADDRESS_SET 0x2A
#define ILI9341_CMD_PAGE_ADDRESS_SET 0x2B

// Pomocná funkcia na poslanie príkazu
void ILI9341_SendCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(DCX_GPIO_Port, DCX_Pin, GPIO_PIN_RESET); // DCX = Command
    HAL_GPIO_WritePin(CSX_GPIO_Port, CSX_Pin, GPIO_PIN_RESET); // CSX = Enable
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CSX_GPIO_Port, CSX_Pin, GPIO_PIN_SET);   // CSX = Disable
}

// Pomocná funkcia na poslanie dát
void ILI9341_SendData(uint8_t* data, size_t size) {
    HAL_GPIO_WritePin(DCX_GPIO_Port, DCX_Pin, GPIO_PIN_SET);   // DCX = Data
    HAL_GPIO_WritePin(CSX_GPIO_Port, CSX_Pin, GPIO_PIN_RESET); // CSX = Enable
    HAL_SPI_Transmit(&hspi1, data, size, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CSX_GPIO_Port, CSX_Pin, GPIO_PIN_SET);   // CSX = Disable
}

// Inicializácia displeja
void ILI9341_Init(void) {
    // Reset displeja
    HAL_GPIO_WritePin(RESX_GPIO_Port, RESX_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(RESX_GPIO_Port, RESX_Pin, GPIO_PIN_SET);
    HAL_Delay(120);

    // Posielanie inicializačných príkazov podľa datasheetu
    uint8_t initCommands[] = {
        0xEF, 3, 0x03, 0x80, 0x02,
        0xCF, 3, 0x00, 0xC1, 0x30,
        0xED, 4, 0x64, 0x03, 0x12, 0x81,
        0xE8, 3, 0x85, 0x00, 0x78,
        0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
        0xF7, 1, 0x20,
        0xEA, 2, 0x00, 0x00,
        0xC0, 1, 0x23, // Power control
        0xC1, 1, 0x10, // Power control
        0xC5, 2, 0x3E, 0x28, // VCOM control
        0xC7, 1, 0x86, // VCOM control
        0x36, 1, 0x48, // Memory Access Control
        0x3A, 1, 0x55, // Pixel Format
        0xB1, 2, 0x00, 0x18, // Frame Control
        0xB6, 3, 0x08, 0x82, 0x27, // Display Function Control
        0xF2, 1, 0x00, // 3Gamma Function Disable
        0x26, 1, 0x01, // Gamma curve selected
        0xE0, 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00, // Positive Gamma
        0xE1, 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, // Negative Gamma
        0x11, 0, // Exit Sleep
        0x29, 0  // Display ON
    };

    for (size_t i = 0; i < sizeof(initCommands);) {
        uint8_t cmd = initCommands[i++];
        uint8_t numArgs = initCommands[i++];
        ILI9341_SendCommand(cmd);
        ILI9341_SendData(&initCommands[i], numArgs);
        i += numArgs;
    }
}

// Vykreslenie bodu
void ILI9341_DrawPixel(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || x >= ILI9341_WIDTH || y < 0 || y >= ILI9341_HEIGHT)
        return;

    uint8_t data[] = {
        (x >> 8) & 0xFF, x & 0xFF, (x >> 8) & 0xFF, x & 0xFF,
        (y >> 8) & 0xFF, y & 0xFF, (y >> 8) & 0xFF, y & 0xFF
    };

    ILI9341_SendCommand(ILI9341_CMD_COLUMN_ADDRESS_SET);
    ILI9341_SendData(data, 4);

    ILI9341_SendCommand(ILI9341_CMD_PAGE_ADDRESS_SET);
    ILI9341_SendData(&data[4], 4);

    ILI9341_SendCommand(ILI9341_CMD_MEMORY_WRITE);
    uint8_t colorData[] = { color >> 8, color & 0xFF };
    ILI9341_SendData(colorData, 2);
}

// Vyplnenie celej obrazovky
void ILI9341_FillScreen(uint16_t color) {
    uint8_t data[] = {
        0, 0, (ILI9341_WIDTH - 1) >> 8, (ILI9341_WIDTH - 1) & 0xFF,
        0, 0, (ILI9341_HEIGHT - 1) >> 8, (ILI9341_HEIGHT - 1) & 0xFF
    };

    ILI9341_SendCommand(ILI9341_CMD_COLUMN_ADDRESS_SET);
    ILI9341_SendData(data, 4);

    ILI9341_SendCommand(ILI9341_CMD_PAGE_ADDRESS_SET);
    ILI9341_SendData(&data[4], 4);

    ILI9341_SendCommand(ILI9341_CMD_MEMORY_WRITE);
    uint8_t colorData[] = { color >> 8, color & 0xFF };
    for (int i = 0; i < ILI9341_WIDTH * ILI9341_HEIGHT; i++) {
        ILI9341_SendData(colorData, 2);
    }
}

// Vykreslenie znaku
void ILI9341_DrawChar(int16_t x, int16_t y, char c, uint16_t color, uint16_t bg, uint8_t size) {
   const  uint8_t *fontData = font[c - 32];
    for (int8_t i = 0; i < 5; i++) {
        uint8_t line = fontData[i];
        for (int8_t j = 0; j < 8; j++) {
            if (line & 0x1) {
                ILI9341_DrawPixel(x + i * size, y + j * size, color);
            } else {
                ILI9341_DrawPixel(x + i * size, y + j * size, bg);
            }
            line >>= 1;
        }
    }
}
