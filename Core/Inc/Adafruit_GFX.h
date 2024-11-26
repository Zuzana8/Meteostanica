/*
 * Adafruit_GFX.h
 *
 *  Created on: Nov 26, 2024
 *      Author: prezentacie
 */

#ifndef ADAFRUIT_GFX_H_
#define ADAFRUIT_GFX_H_

#include <stdint.h>

// Základné funkcie
void GFX_Init(void); // Inicializácia knižnice
void GFX_FillScreen(uint16_t color); // Vyplnenie celej obrazovky jednou farbou
void GFX_DrawPixel(int16_t x, int16_t y, uint16_t color); // Kreslenie bodu
void GFX_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color); // Kreslenie čiary
void GFX_SetCursor(int16_t x, int16_t y); // Nastavenie kurzora
void GFX_SetTextColor(uint16_t color); // Nastavenie farby textu
void GFX_Print(const char* text); // Tlač textu

// Farby (definície pre jednoduché použitie)
#define COLOR_BLACK  0x0000
#define COLOR_RED    0xF800
#define COLOR_GREEN  0x07E0
#define COLOR_BLUE   0x001F
#define COLOR_WHITE  0xFFFF

#endif /* ADAFRUIT_GFX_H_ */
