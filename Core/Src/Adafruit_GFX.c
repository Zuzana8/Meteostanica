/*
 * Adafruit_GFX.c
 *
 *  Created on: Nov 26, 2024
 *      Author: prezentacie
 */

#include "Adafruit_GFX.h"
#include "ILI9341.h" // Ovládač pre váš TFT displej

// Premenné pre kurzor a farbu textu
static int16_t cursor_x = 0, cursor_y = 0;
static uint16_t text_color = COLOR_WHITE;

// Inicializácia GFX knižnice
void GFX_Init(void) {
    ILI9341_Init(); // Inicializácia ovládača displeja
    GFX_FillScreen(COLOR_BLACK); // Vyčistenie obrazovky
}

// Vyplnenie celej obrazovky jednou farbou
void GFX_FillScreen(uint16_t color) {
    ILI9341_FillScreen(color);
}

// Kreslenie jedného bodu
void GFX_DrawPixel(int16_t x, int16_t y, uint16_t color) {
    ILI9341_DrawPixel(x, y, color);
}

// Kreslenie čiary (jednoduchý Bresenhamov algoritmus)
void GFX_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    int16_t dx = abs(x1 - x0), dy = abs(y1 - y0);
    int16_t sx = (x0 < x1) ? 1 : -1;
    int16_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx - dy;

    while (1) {
        GFX_DrawPixel(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        int16_t e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x0 += sx; }
        if (e2 < dx) { err += dx; y0 += sy; }
    }
}

// Nastavenie kurzora
void GFX_SetCursor(int16_t x, int16_t y) {
    cursor_x = x;
    cursor_y = y;
}

// Nastavenie farby textu
void GFX_SetTextColor(uint16_t color) {
    text_color = color;
}

// Tlač textu
void GFX_Print(const char* text) {
    while (*text) {
        ILI9341_DrawChar(cursor_x, cursor_y, *text, text_color, COLOR_BLACK, 1);
        cursor_x += 6; // Posunutie kurzora (prispôsobte veľkosť fontu)
        text++;
    }
}
