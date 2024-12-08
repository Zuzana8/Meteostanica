/*
 * zambretti.c
 *
 *  Created on: Dec 1, 2024
 *      Author: Klaudia Piwkova
 */

float zero_pressure = 0;
float old_pressure = 0;
float elevation = ELEVATION_BA;

float set_elevation(float e) {
	elevation = e;
}

float zambretti_get_zero_pressure(float pressure, float temperature) {
	zero_pressure = pressure * pow(1 - (0.0065 * elevation) / (temperature + (0.0065 * elevation) + 273.15),-5.255 );
	return zero_pressure;
}

//change of altitude in mbar
TREND_TYPE count_pressure_trend(float past, float now){
	float delta = now - past;
	if (delta > 1.6) {
		return RISING;
	} else if (delta < 1.6) {
		return FALLING;
	} else {
		return STEADY;
	}
}

uint8_t zambretti_count_zNumber(float pressure, TREND_TYPE trend) {
	uint8_t zNumber = 0;
	switch  (trend) {
	case STEADY:
		zNumber = round(127 - 0.12 * pressure);
		break;
	case RISING:
		zNumber = round(144 - 0.13 * pressure);
		break;
	case FALLING:
		zNumber = round(185 - 0.16 * pressure);
		break;
	}

	if (zNumber > 32) {
		return 32;
	}
	if (zNumber < 1) {
			return 1;
	}
	return zNumber;
}

void zambretti_get_text(uint8_t zNumber, char *text) {
    if (text == NULL) {
        return; // Ensure text pointer is valid
    }

    switch (zNumber) {
        case 1: strcpy(text, "Settled Fine"); break;
        case 2: strcpy(text, "Fine Weather"); break;
        case 3: strcpy(text, "Fine Becoming Less Settled"); break;
        case 4: strcpy(text, "Fairly Fine Showery Later"); break;
        case 5: strcpy(text, "Showery Becoming more unsettled"); break;
        case 6: strcpy(text, "Unsettled, Rain later"); break;
        case 7: strcpy(text, "Rain at times, worse later."); break;
        case 8: strcpy(text, "Rain at times, becoming very unsettled"); break;
        case 9: strcpy(text, "Very Unsettled, Rain"); break;
        case 10: strcpy(text, "Settled Fine"); break;
        case 11: strcpy(text, "Fine Weather"); break;
        case 12: strcpy(text, "Fine, Possibly showers"); break;
        case 13: strcpy(text, "Fairly Fine , Showers likely"); break;
        case 14: strcpy(text, "Showery Bright Intervals"); break;
        case 15: strcpy(text, "Changeable some rain"); break;
        case 16: strcpy(text, "Unsettled, rain at times"); break;
        case 17: strcpy(text, "Rain at Frequent Intervals"); break;
        case 18: strcpy(text, "Very Unsettled, Rain"); break;
        case 19: strcpy(text, "Stormy, much rain"); break;
        case 20: strcpy(text, "Settled Fine"); break;
        case 21: strcpy(text, "Fine Weather"); break;
        case 22: strcpy(text, "Becoming Fine"); break;
        case 23: strcpy(text, "Fairly Fine, Improving"); break;
        case 24: strcpy(text, "Fairly Fine, Possibly showers, early"); break;
        case 25: strcpy(text, "Showery Early, Improving"); break;
        case 26: strcpy(text, "Changeable Mending"); break;
        case 27: strcpy(text, "Rather Unsettled Clearing Later"); break;
        case 28: strcpy(text, "Unsettled, Probably Improving"); break;
        case 29: strcpy(text, "Unsettled, short fine Intervals"); break;
        case 30: strcpy(text, "Very Unsettled, Finer at times"); break;
        case 31: strcpy(text, "Stormy, possibly improving"); break;
        case 32: strcpy(text, "Stormy, much rain"); break;
        default: strcpy(text, "Invalid value"); break;
    }
}

void zambretti(float pressure, float temperature, char* text) {
	zambretti_get_zero_pressure;
	TREND_TYPE trend = count_pressure_trend(old_pressure, zero_pressure);
	uint8_t z = zambretti_count_zNumber(zero_pressure, trend);
	zambretti_get_text(z, text);
	old_pressure = zero_pressure;
}




