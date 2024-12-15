/*
 * zambretti.h
 *
 *  Created on: Dec 1, 2024
 *      Author: Klaudia Piwkova
 */

#ifndef ZAMBRETTI_H_
#define ZAMBRETTI_H_

#include <stdint.h>
#include <string.h>
#include <math.h>

#define ELEVATION_BA 140;

typedef enum TREND_TYPE{
    STEADY = 0,
    RISING = 1,
    FALLING = 2
} TREND_TYPE;

void set_elevation(float e);

float zambretti_get_zero_pressure(float pressure, float temmperature);

TREND_TYPE count_pressure_trend(float past, float now);

uint8_t zambretti_count_zNumber(float pressure, TREND_TYPE trend);

// longest text 50 >> char text_buffer[50]
void zambretti_get_text(uint8_t zNumber, char *text);

void zambretti(float pressure, float temperature, char *text);

#endif /* ZAMBRETTI_H_ */
