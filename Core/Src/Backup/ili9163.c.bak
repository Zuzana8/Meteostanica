#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ili9163.h"
#include "Minimum_font.h"
#include "spi.h"

// Global variables for display dimensions
uint16_t DISPLAY_WIDTH = 320;
uint16_t DISPLAY_HEIGHT = 240;

// Low-level LCD driving functions --------------------------------------------------------------------------

// Reset the LCD hardware
void lcdReset(void)
{
    // Reset pin is active low (0 = reset, 1 = ready)
    res_reset();
    LL_mDelay(100);

    res_set();
    LL_mDelay(100);
}

void lcdWriteCommand(uint8_t address)
{
    cd_reset();
    cs_reset();
    readWriteSPI1(address);
    cs_set();
}

void lcdWriteParameter(int16_t parameter)
{
    cd_set();
    cs_reset();
    readWriteSPI1(parameter & 0xFF);
    if (parameter > 0xFF)
    {
        readWriteSPI1((uint8_t)(parameter >> 8));
    }
    cs_set();
}

void lcdWriteData(uint8_t dataByte1, uint8_t dataByte2)
{
    cd_set();
    cs_reset();
    readWriteSPI1(dataByte1);
    readWriteSPI1(dataByte2);
    cs_set();
}

void lcdInitialise(uint8_t orientation)
{
    lcdReset();

    lcdWriteCommand(EXIT_SLEEP_MODE);
    LL_mDelay(100);

    lcdWriteCommand(SET_PIXEL_FORMAT);
    lcdWriteParameter(0x05);

    lcdWriteCommand(SET_GAMMA_CURVE);
    lcdWriteParameter(0x04);

    lcdWriteCommand(GAM_R_SEL);
    lcdWriteParameter(0x01);

    lcdWriteCommand(POSITIVE_GAMMA_CORRECT);
    lcdWriteParameter(0x3f);
    lcdWriteParameter(0x25);
    lcdWriteParameter(0x1c);
    lcdWriteParameter(0x1e);
    lcdWriteParameter(0x20);
    lcdWriteParameter(0x12);
    lcdWriteParameter(0x2a);
    lcdWriteParameter(0x90);
    lcdWriteParameter(0x24);
    lcdWriteParameter(0x11);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00);

    lcdWriteCommand(NEGATIVE_GAMMA_CORRECT);
    lcdWriteParameter(0x20);
    lcdWriteParameter(0x20);
    lcdWriteParameter(0x20);
    lcdWriteParameter(0x20);
    lcdWriteParameter(0x05);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x15);
    lcdWriteParameter(0xa7);
    lcdWriteParameter(0x3d);
    lcdWriteParameter(0x18);
    lcdWriteParameter(0x25);
    lcdWriteParameter(0x2a);
    lcdWriteParameter(0x2b);
    lcdWriteParameter(0x2b);
    lcdWriteParameter(0x3a);

    lcdWriteCommand(FRAME_RATE_CONTROL1);
    lcdWriteParameter(0x08);
    lcdWriteParameter(0x08);

    lcdWriteCommand(DISPLAY_INVERSION);
    lcdWriteParameter(0x07);

    lcdWriteCommand(POWER_CONTROL1);
    lcdWriteParameter(0x0a);
    lcdWriteParameter(0x02);

    lcdWriteCommand(POWER_CONTROL2);
    lcdWriteParameter(0x02);

    lcdWriteCommand(VCOM_CONTROL1);
    lcdWriteParameter(0x50);
    lcdWriteParameter(0x5b);

    lcdWriteCommand(VCOM_OFFSET_CONTROL);
    lcdWriteParameter(0x40);

    lcdWriteCommand(SET_COLUMN_ADDRESS);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00);
    lcdWriteParameter(DISPLAY_WIDTH >> 8);
    lcdWriteParameter(DISPLAY_WIDTH & 0xFF);

    lcdWriteCommand(SET_PAGE_ADDRESS);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00);
    lcdWriteParameter(DISPLAY_HEIGHT >> 8);
    lcdWriteParameter(DISPLAY_HEIGHT & 0xFF);

    lcdWriteCommand(SET_ADDRESS_MODE);
    lcdWriteParameter(orientation);

    lcdWriteCommand(SET_DISPLAY_ON);
    lcdWriteCommand(WRITE_MEMORY_START);
}

// LCD graphics functions -----------------------------------------------------------------------------------

void lcdClearDisplay(uint16_t colour)
{
    uint32_t pixel;

    lcdWriteCommand(SET_COLUMN_ADDRESS);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00);
    lcdWriteParameter(DISPLAY_WIDTH >> 8);
    lcdWriteParameter(DISPLAY_WIDTH & 0xFF);

    lcdWriteCommand(SET_PAGE_ADDRESS);
    lcdWriteParameter(0x00);
    lcdWriteParameter(0x00);
    lcdWriteParameter(DISPLAY_HEIGHT >> 8);
    lcdWriteParameter(DISPLAY_HEIGHT & 0xFF);

    lcdWriteCommand(WRITE_MEMORY_START);
    for (pixel = 0; pixel < DISPLAY_WIDTH * DISPLAY_HEIGHT; pixel++)
    {
        lcdWriteData(colour >> 8, colour & 0xFF);
    }
}

void lcdPlot(int16_t x, int16_t y, uint16_t colour)
{
    if (x >= 0 && x < DISPLAY_WIDTH && y >= 0 && y < DISPLAY_HEIGHT)
    {
        lcdWriteCommand(SET_COLUMN_ADDRESS);
        lcdWriteParameter(x >> 8);
        lcdWriteParameter(x & 0xFF);
        lcdWriteParameter(x >> 8);
        lcdWriteParameter(x & 0xFF);

        lcdWriteCommand(SET_PAGE_ADDRESS);
        lcdWriteParameter(y >> 8);
        lcdWriteParameter(y & 0xFF);
        lcdWriteParameter(y >> 8);
        lcdWriteParameter(y & 0xFF);

        lcdWriteCommand(WRITE_MEMORY_START);
        lcdWriteData(colour >> 8, colour & 0xFF);
    }
}

void lcdLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour) {
    int16_t dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int16_t dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int16_t err = dx + dy, e2;

    while (1) {
        lcdPlot(x0, y0, colour);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void lcdFilledCircle(int16_t x0, int16_t y0, int16_t r, uint16_t colour) {
//    int16_t x = -r, y = 0, err = 2 - 2 * r;
//    do {
//        lcdLine(x0 - x, y0 - y, x0 + x, y0 - y, colour); // Top horizontal line
//        lcdLine(x0 - x, y0 + y, x0 + x, y0 + y, colour); // Bottom horizontal line
//        if ((err += y++ * 2 + 1) > 0) err += ++x * 2 + 1;
//    } while (x < 0);
	drawCircleZone(x0-r, y0-r, x0+r, y0+r, x0, y0, r, colour);
}

// Function to draw a circular zone within a rectangular area
void drawCircleZone(int16_t x1, int16_t y1, int16_t x2, int16_t y2,
                    int16_t centerX, int16_t centerY, int16_t radius,
                    uint16_t insideColour) {
    for (int16_t y = y1; y <= y2; y++) { // Iterate over rows
        for (int16_t x = x1; x <= x2; x++) { // Iterate over columns
            // Calculate the squared distance from the circle's center
            int32_t dx = x - centerX;
            int32_t dy = y - centerY;
            float distance = sqrt((dx * dx) + (dy * dy));

            // Determine pixel colour based on the distance
            if (distance <= radius) {
                lcdPlot(x, y, insideColour); // Inside circle
            }
        }
    }
}

void lcdRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour)
{
    lcdLine(x0, y0, x0, y1, colour);
    lcdLine(x0, y1, x1, y1, colour);
    lcdLine(x1, y0, x1, y1, colour);
    lcdLine(x0, y0, x1, y0, colour);
}

void lcdFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t colour)
{
    uint32_t pixels;
    int16_t temp;

    if (x0 > x1) { temp = x0; x0 = x1; x1 = temp; }
    if (y0 > y1) { temp = y0; y0 = y1; y1 = temp; }

    lcdWriteCommand(SET_COLUMN_ADDRESS);
    lcdWriteParameter(x0 >> 8);
    lcdWriteParameter(x0 & 0xFF);
    lcdWriteParameter(x1 >> 8);
    lcdWriteParameter(x1 & 0xFF);

    lcdWriteCommand(SET_PAGE_ADDRESS);
    lcdWriteParameter(y0 >> 8);
    lcdWriteParameter(y0 & 0xFF);
    lcdWriteParameter(y1 >> 8);
    lcdWriteParameter(y1 & 0xFF);

    lcdWriteCommand(WRITE_MEMORY_START);
    for (pixels = 0; pixels < ((x1 - x0 + 1) * (y1 - y0 + 1)); pixels++)
    {
        lcdWriteData(colour >> 8, colour & 0xFF);
    }
}

void lcdPutCh(unsigned char character, uint16_t x, uint16_t y, uint16_t fgColour, uint16_t bgColour)
{
    uint8_t row, column;

    // Nastavenie rozsahu na displeji pre vykreslenie znaku (6x16)
    lcdWriteCommand(SET_COLUMN_ADDRESS);
    lcdWriteParameter(x >> 8);
    lcdWriteParameter(x & 0xFF);
    lcdWriteParameter((x + 5) >> 8);
    lcdWriteParameter((x + 5) & 0xFF);

    lcdWriteCommand(SET_PAGE_ADDRESS);
    lcdWriteParameter(y >> 8);
    lcdWriteParameter(y & 0xFF);
    lcdWriteParameter((y + 15) >> 8);
    lcdWriteParameter((y + 15) & 0xFF);

    lcdWriteCommand(WRITE_MEMORY_START);

    // Vykresli dáta fontu znaku
    for (row = 0; row < 8; row++)
    {
        for (column = 0; column < 6; column++)
        {
            // Namiesto zrkadlenia stĺpcov použijeme priamy prístup k matici fontu
            if ((fontus[character][column]) & (1 << row))
            {
                lcdWriteData(fgColour >> 8, fgColour & 0xFF);
            }
            else
            {
                lcdWriteData(bgColour >> 8, bgColour & 0xFF);
            }
        }
    }
}


void lcdPutS(const char *string, uint16_t x, uint16_t y, uint16_t fgColour, uint16_t bgColour)
{
    uint16_t origin = x;
    size_t length = strlen(string);

    for (size_t characterNumber = 0; characterNumber < length; characterNumber++)
    {
        if (x > DISPLAY_WIDTH - 12)
        {
            x = origin;
            y += 16;
        }

        if (y > DISPLAY_HEIGHT - 16) break;

        lcdPutCh(string[characterNumber], x, y, fgColour, bgColour);
        x += 6;
    }
}

uint16_t decodeRgbValue(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}


void lcdPutLargeS(const char *string, uint16_t x, uint16_t y, uint16_t fgColour, uint16_t bgColour)
{
    uint16_t origin = x;
    size_t length = strlen(string);

    for (size_t characterNumber = 0; characterNumber < length; characterNumber++)
    {
        if (x > DISPLAY_WIDTH - 24)
        {
            x = origin;
            y += 16;
        }

        if (y > DISPLAY_HEIGHT - 16) break;

        lcdPutLargeCh(string[characterNumber], x, y, fgColour, bgColour);
        x += 12;
    }
}


void lcdPutLargeCh(unsigned char character, uint16_t x, uint16_t y, uint16_t fgColour, uint16_t bgColour)
{
    uint8_t row, column;

    // Set the area for the character (12x16)
    lcdWriteCommand(SET_COLUMN_ADDRESS);
    lcdWriteParameter(x >> 8);
    lcdWriteParameter(x & 0xFF);
    lcdWriteParameter((x + 11) >> 8);
    lcdWriteParameter((x + 11) & 0xFF);

    lcdWriteCommand(SET_PAGE_ADDRESS);
    lcdWriteParameter(y >> 8);
    lcdWriteParameter(y & 0xFF);
    lcdWriteParameter((y + 15) >> 8);
    lcdWriteParameter((y + 15) & 0xFF);

    lcdWriteCommand(WRITE_MEMORY_START);

    // Plot the font data for larger character (16 rows)
    for (row = 0; row < 16; row++)
    {
        for (column = 0; column < 12; column++)
        {
            if ((fontus[character][column / 2]) & (1 << row))
            {
                lcdWriteData(fgColour >> 8, fgColour & 0xFF);
            }
            else
            {
                lcdWriteData(bgColour >> 8, bgColour & 0xFF);
            }
        }
    }
}

void display_time(void)
{
	char buffer[20];
	snprintf(buffer, sizeof(buffer), "Time: %02d:%02d:%02d", sTime.Hours, sTime.Minutes, sTime.Seconds);

	// Clear a larger area to make sure there is no leftover text
	//lcdFilledRectangle(10, 10, 200, 30, 0xFFFF); // Adjusted dimensions to clear more space

	// Draw the current time on the display
	lcdPutS(buffer, 10, 10, 0x0000, 0xFFFF);  // Black text on white background
}




void display_time_with_large_digits(void)
{
    char buffer[20];

    // Display the "Time:" label on the first line
    snprintf(buffer, sizeof(buffer), "Time:");
    lcdPutLargeS(buffer, 10, 10, 0x0000, 0xFFFF);  // Normal size "Time:" label in black text on white background

    // Display the large time on the next line
    snprintf(buffer, sizeof(buffer), "%02d:%02d", sTime.Hours, sTime.Minutes);

    // Clear area for larger time display
    //lcdFilledRectangle(10, 40, 230, 90, 0xFFFF);  // Adjust dimensions to clear space for larger text

    // Draw the current time in larger font on the display
    lcdPutLargeS(buffer, 10, 40, 0x0000, 0xFFFF);  // Black text on white background
}

void drawSunnySymbol(uint16_t x, uint16_t y, uint16_t fgColour, uint16_t bgColour) {
    // Fill the background area for the sunny symbol
    //lcdFilledRectangle(x - 25, y - 25, x + 25, y + 25, bgColour);

    // Draw the sun (circle)
    lcdFilledCircle(x, y, 10, fgColour); // Circle center (x, y), radius 10

    // Draw rays
    lcdLine(x - 15, y, x - 25, y, fgColour);  // Left ray
    lcdLine(x + 15, y, x + 25, y, fgColour);  // Right ray
    lcdLine(x, y - 15, x, y - 25, fgColour);  // Top ray
    lcdLine(x, y + 15, x, y + 25, fgColour);  // Bottom ray
    lcdLine(x - 10, y - 10, x - 18, y - 18, fgColour); // Top-left ray
    lcdLine(x + 10, y - 10, x + 18, y - 18, fgColour); // Top-right ray
    lcdLine(x - 10, y + 10, x - 18, y + 18, fgColour); // Bottom-left ray
    lcdLine(x + 10, y + 10, x + 18, y + 18, fgColour); // Bottom-right ray
}


void drawCloudySymbol(uint16_t x, uint16_t y, uint16_t fgColour, uint16_t bgColour) {
    // Fill the background area for the cloudy symbol
    //lcdFilledRectangle(x - 25, y - 15, x + 25, y + 25, bgColour);

    // Draw the main cloud body (three overlapping circles)
    lcdFilledCircle(x, y, 12, fgColour);         // Center circle
    lcdFilledCircle(x - 15, y + 5, 10, fgColour); // Left circle
    lcdFilledCircle(x + 15, y + 5, 10, fgColour); // Right circle
}


void drawRainySymbol(uint16_t x, uint16_t y, uint16_t fgColour, uint16_t bgColour) {
    // Fill the background area for the rainy symbol
    //lcdFilledRectangle(x - 25, y - 15, x + 25, y + 35, bgColour);

    // Draw the cloud
    drawCloudySymbol(x, y, fgColour, bgColour);

    // Draw raindrops (lines beneath the cloud)
    lcdLine(x - 10, y + 15, x - 10, y + 25, fgColour); // Left raindrop
    lcdLine(x, y + 15, x, y + 25, fgColour);           // Center raindrop
    lcdLine(x + 10, y + 15, x + 10, y + 25, fgColour); // Right raindrop
}

void drawWeatherSymbol(uint16_t x, uint16_t y, uint8_t symbolIndex, uint16_t fgColour, uint16_t bgColour) {
    if (symbolIndex >= FONTUS_SIZE) return;

    lcdPutCh(symbolIndex, x, y, fgColour, bgColour);
}

void drawWeather(uint8_t currentWeather) {
    // Neutral color for non-highlighted symbols
    uint16_t neutralColor = decodeRgbValue(0, 0, 0); // White foreground

    // Draw sunny symbol (highlight if currentWeather is SUNNY_SYMBOL_INDEX)
    drawSunnySymbol(160, 20,
                     currentWeather == SUNNY_SYMBOL_INDEX ? decodeRgbValue(255, 255, 0) : neutralColor, // Yellow if sunny
                     decodeRgbValue(0xFF, 0xFF, 0xFF)); // White background

    // Draw cloudy symbol (highlight if currentWeather is CLOUDY_SYMBOL_INDEX)
    drawCloudySymbol(220, 20,
                    currentWeather == CLOUDY_SYMBOL_INDEX ? decodeRgbValue(170, 170, 170) : neutralColor, // Gray if cloudy
                    decodeRgbValue(0xFF, 0xFF, 0xFF)); // White background

    // Draw rainy symbol (highlight if currentWeather is RAINY_SYMBOL_INDEX)
    drawRainySymbol(280, 20,
                    currentWeather == RAINY_SYMBOL_INDEX ? decodeRgbValue(0, 80, 120) : neutralColor, // Blue if rainy
                    decodeRgbValue(0xFF, 0xFF, 0xFF)); // White background
}


//void drawTemperatureGraph(float *history, uint16_t numPoints) {
//    // Clear the display
//    lcdClearDisplay(decodeRgbValue(255, 255, 255)); // Clear to white background
//
//    // Draw the graph axes
//    lcdLine(20, 200, 300, 200, decodeRgbValue(0, 0, 0)); // X-axis
//    lcdLine(20, 20, 20, 200, decodeRgbValue(0, 0, 0));   // Y-axis
//
//    // Draw temperature graph
//    float maxTemp = 40.0; // Max temperature for scaling
//    float minTemp = -10.0; // Min temperature for scaling
//    uint16_t graphWidth = 280;
//    uint16_t graphHeight = 180;
//
//    for (uint16_t i = 0; i < numPoints - 1; i++) {
//        uint16_t x1 = 20 + (i * graphWidth) / (numPoints - 1);
//        uint16_t y1 = 200 - ((history[i] - minTemp) * graphHeight) / (maxTemp - minTemp);
//        uint16_t x2 = 20 + ((i + 1) * graphWidth) / (numPoints - 1);
//        uint16_t y2 = 200 - ((history[i + 1] - minTemp) * graphHeight) / (maxTemp - minTemp);
//
//        lcdLine(x1, y1, x2, y2, decodeRgbValue(255, 0, 0)); // Red line for graph
//    }
//
//    // Add labels for axes
//    lcdPutS("Hours", 140, 210, decodeRgbValue(0, 0, 0), decodeRgbValue(255, 255, 255));
//    lcdPutS("Temp", 5, 10, decodeRgbValue(0, 0, 0), decodeRgbValue(255, 255, 255));
//}

void drawTemperatureGraph(float *history, uint16_t numPoints) {
    // Clear the display
    lcdClearDisplay(decodeRgbValue(255, 255, 255)); // Clear to white background

    // Draw the graph axes
    lcdLine(20, 200, 300, 200, decodeRgbValue(0, 0, 0)); // X-axis
    lcdLine(20, 20, 20, 200, decodeRgbValue(0, 0, 0));   // Y-axis

    // Graph parameters
    float maxTemp = 40.0;  // Max temperature for scaling
    float minTemp = -10.0; // Min temperature for scaling
    uint16_t graphWidth = 280;  // Total width of the graph area
    uint16_t graphHeight = 180; // Total height of the graph area
    uint16_t measurementsPerDay = 8; // 3-hour intervals (8 measurements per day)

    // Draw temperature graph
    for (uint16_t i = 0; i < numPoints - 1; i++) {
        uint16_t x1 = 20 + (i * graphWidth) / (numPoints - 1);
        uint16_t y1 = 200 - ((history[i] - minTemp) * graphHeight) / (maxTemp - minTemp);
        uint16_t x2 = 20 + ((i + 1) * graphWidth) / (numPoints - 1);
        uint16_t y2 = 200 - ((history[i + 1] - minTemp) * graphHeight) / (maxTemp - minTemp);

        lcdLine(x1, y1, x2, y2, decodeRgbValue(255, 0, 0)); // Red line for graph
    }

    // Add X-axis labels (distributed across the full width)
    for (uint16_t i = 0; i <= 7; i++) { // 7 days of history
        uint16_t x = 20 + (i * graphWidth) / 7; // Distribute evenly across the graph
        char label[12];

        // Reverse the labels so the leftmost is -168 and the rightmost is 0
        int hoursAgo = (7 - i) * 24;
        snprintf(label, sizeof(label), "-%dh", hoursAgo); // Hours ago (e.g., -168h to -24h)
        if (hoursAgo == 0) {
            snprintf(label, sizeof(label), "0h"); // Special case for 0h
        }

        lcdPutS(label, x - 10, 210, decodeRgbValue(0, 0, 0), decodeRgbValue(255, 255, 255)); // Adjust X label positioning
    }


    // Add Y-axis labels (temperature)
    for (int temp = minTemp; temp <= maxTemp; temp += 10) { // Labels every 10 degrees
        uint16_t y = 200 - ((temp - minTemp) * graphHeight) / (maxTemp - minTemp);
        char label[5];
        snprintf(label, sizeof(label), "%d", temp); // Y-axis values (temperature)
        lcdPutS(label, 0, y - 5, decodeRgbValue(0, 0, 0), decodeRgbValue(255, 255, 255)); // Adjust Y label positioning
    }

    // Add axis labels
    lcdPutS("Hours Ago", 140, 220, decodeRgbValue(0, 0, 0), decodeRgbValue(255, 255, 255));
    lcdPutS("Temp", 5, 10, decodeRgbValue(0, 0, 0), decodeRgbValue(255, 255, 255));
}






