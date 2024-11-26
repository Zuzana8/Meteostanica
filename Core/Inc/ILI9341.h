/*
 * ILI9341.h
 *
 *  Created on: Nov 26, 2024
 *      Author: prezentacie
 */

#ifndef ILI9341_H_
#define ILI9341_H_

#include <stdint.h>

// Inicializácia displeja
void ILI9341_Init(void);

// Kreslenie bodu
void ILI9341_DrawPixel(int16_t x, int16_t y, uint16_t color);

// Vyplnenie obrazovky
void ILI9341_FillScreen(uint16_t color);

// Kreslenie znaku (fontová logika)
void ILI9341_DrawChar(int16_t x, int16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);

#endif /* ILI9341_H_ */

