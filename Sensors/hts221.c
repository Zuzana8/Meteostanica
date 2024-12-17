/*
 * hts221.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Piwkova
 */
#include "hts221.h"


float HTS221_TemperatureSlope;
float HTS221_TemperatureZero;
float HTS221_HumiditySlope;
float HTS221_HumidityZero;

//initialisation of sensor
uint8_t HTS221_init() {
	uint8_t val = hts221_read_byte(HTS221_WHO_AM_I);

	if(val == HTS221_WHO_AM_I_VALUE) {
		//control register setup
		uint8_t ctrl_reg1 = 0b10000011;
		hts221_write_byte(HTS221_CTRL_REG1, ctrl_reg1);

		hts221_get_temperature_calibration();
		return 1;
	}
	return 0;
}

void hts221_write_byte(uint8_t reg_addr, uint8_t value) {
	i2c_master_write(value, reg_addr, HTS221_I2C_ADDRESS);
}

uint8_t hts221_read_byte(uint8_t reg_addr) {
	uint8_t val = 0;
	i2c_master_read(&val, 1, reg_addr, HTS221_I2C_ADDRESS);
	return val;
}

//alternates method read bytes
void HTS221_read_bytes(uint8_t reg_addr, uint8_t* values, size_t length) {
    i2c_master_read(values, length, reg_addr, HTS221_I2C_ADDRESS);
    // Added a delay or a check for 'end_of_read_flag' to ensure data has been read before proceeding
}

void hts221_get_temperature_calibration() {
    uint8_t calibration_data[4]; // Buffer to calibration values
    uint8_t t_out_data[4]; // Buffer to hold T0_OUT/T1_OUT calibration values
    int16_t t0_out, t1_out;
    uint16_t t0_degC, t1_degC;

    // Read T0_degC, T1_degC and MSB bits together with auto-increment
    HTS221_read_bytes(T0_degC_x8 | 0x80, calibration_data, 4);

    // Extract the calibration values
    t0_degC = ((uint16_t)(calibration_data[0]) | ((uint16_t)(calibration_data[2] & 0x03) << 8));
    t1_degC = ((uint16_t)(calibration_data[1]) | ((uint16_t)(calibration_data[2] & 0x0C) << 8));

    // Read T0_OUT and T1_OUT together with auto-increment
    HTS221_read_bytes(T0_OUT_1 | 0x80, t_out_data, 4);

    // Combine the bytes into the T0_OUT and T1_OUT values
    t0_out = (int16_t)(t_out_data[1] << 8 | t_out_data[0]);
    t1_out = (int16_t)(t_out_data[3] << 8 | t_out_data[2]);

    // Calculate the temperature calibration slope and intercept using the calibration values
    HTS221_TemperatureSlope = (t1_degC - t0_degC) / (8.0 * t1_out - t0_out);
    HTS221_TemperatureZero = (t0_degC / 8.0) - HTS221_TemperatureSlope * t0_out;
    LL_mDelay(100);
}

float hts221_get_temperature() {
    uint8_t t_out_buffer[2]; // Buffer to hold the two temperature bytes
    int16_t t_out;
    float temp;

    // Read both temperature registers at once
    HTS221_read_bytes(HTS221_TEMP_OUT_L | 0x80, t_out_buffer, 2);

    // Combine the two bytes into a single 16-bit value
    t_out = (int16_t)((uint16_t)t_out_buffer[1] << 8 | t_out_buffer[0]);

    // Calculate the temperature in Celsius
    temp = (float)((t_out * -HTS221_TemperatureSlope) + HTS221_TemperatureZero);
    return temp;
}

float hts221_get_humidity() {

	uint8_t buffer[2];
	float   hum;

	HTS221_read_bytes(H0_rH_x2 | 0x80, buffer, 2);
	int16_t h0_rh = buffer[0] >> 1;
	int16_t h1_rh = buffer[1] >> 1;

	HTS221_read_bytes(H0_OUT_1 | 0x80, buffer, 2);
	int16_t h0_t0_out = (((uint16_t)buffer[1]) << 8) | (uint16_t)buffer[0];

	HTS221_read_bytes(H1_OUT_1 | 0x80, buffer, 2);
	int16_t h1_t0_out = (((uint16_t)buffer[1]) << 8) | (uint16_t)buffer[0];

	HTS221_read_bytes(HTS221_HUM_OUT_L | 0x80, buffer, 2);
	int16_t h_out = (((uint16_t)buffer[1]) << 8) | (uint16_t)buffer[0];

	//hum = (float)(H_T_out - H0_T0_out) * (float)(H1_rh - H0_rh) / (float)(H1_T0_out - H0_T0_out)  +  H0_rh;
	hum = (float) (h0_rh + ((h1_rh-h0_rh) * ((float)h_out - (float)h0_t0_out)/((float)h1_t0_out-(float)h0_t0_out)));

	if (hum > 100.0) {
	   hum = 100.0;
	}
	return hum;
}
