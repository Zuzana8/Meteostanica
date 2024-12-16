/*
 * hts221.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Piwkova
 */
#include "hts221.h"

float HTS221_TemperatureSlope;
float HTS221_TemperatureZero;


//initialisation of sensor
uint8_t HTS221_init() {
	uint8_t val = hts221_read_byte(HTS221_WHO_AM_I);

	if(val == HTS221_WHO_AM_I_VALUE) {
		//control register setup
		uint8_t ctrl_reg1 = 0b10000011;
		hts221_write_byte(HTS221_CTRL_REG1, ctrl_reg1);

		//for the other method of calculating temperature
	    HTS221_get_temperature_calibration();

		return 1;
	}

	return 0;
}

//alternates method read bytes
void HTS221_read_bytes(uint8_t reg_addr, uint8_t* values, size_t length) {
    i2c_master_read(values, length, reg_addr, HTS221_I2C_ADDRESS);
    // Added a delay or a check for 'end_of_read_flag' to ensure data has been read before proceeding

}

void hts221_write_byte(uint8_t reg_addr, uint8_t value) {
	i2c_master_write(value, reg_addr, HTS221_I2C_ADDRESS);
}

uint8_t hts221_read_byte(uint8_t reg_addr) {
	uint8_t val = 0;
	i2c_master_read(&val, 1, reg_addr, HTS221_I2C_ADDRESS);
	return val;
}

void hts221_read_array(uint8_t *val, uint8_t reg_addr, uint8_t len) {
	i2c_master_read(val, len, reg_addr, HTS221_I2C_ADDRESS);
}

float hts221_get_temperature() {

	float temp;

	//calibration registers
	uint8_t msb = hts221_read_byte(MSB);

	uint8_t t0_x8 = hts221_read_byte(T0_degC_x8);
	uint8_t t1_x8 = hts221_read_byte(T1_degC_x8);

	float T0_degC = (float) (t0_x8+(1 << 6)*(msb & 0x03))/8.0;
	float T1_degC = (float) (t1_x8+(1 << 6)*(msb & 0x0C))/8.0;

	uint16_t t0_out = (hts221_read_byte(T0_OUT_1) << 8) | hts221_read_byte(T0_OUT_2);
	uint16_t t1_out = (hts221_read_byte(T1_OUT_1) << 8) | hts221_read_byte(T1_OUT_2);

    //output value
	uint16_t t_out = hts221_read_byte(HTS221_TEMP_OUT_L) | (hts221_read_byte(HTS221_TEMP_OUT_H) << 8);

    //calculate temperature
	temp = (T1_degC + (((float)t_out-(float)t1_out) * (T1_degC - T0_degC)/((float)t1_out-(float)t0_out)));
	return temp;
}

void HTS221_get_temperature_calibration(void) {
    uint8_t calibration_data[4]; // Buffer to hold T0/T1 calibration values
    uint8_t t_out_data[4]; // Buffer to hold T0_OUT/T1_OUT calibration values
    int16_t t0_out, t1_out;
    uint16_t t0_degC, t1_degC;

    // Read T0_degC, T1_degC and MSB bits together with auto-increment
    HTS221_read_bytes(HTS221_T0_degC_x8 | 0x80, calibration_data, 4);

    // Extract the calibration values
    t0_degC = ((uint16_t)(calibration_data[0]) | ((uint16_t)(calibration_data[2] & 0x03) << 8));
        t1_degC = ((uint16_t)(calibration_data[1]) | ((uint16_t)(calibration_data[2] & 0x0C) << 8));

    // Read T0_OUT and T1_OUT together with auto-increment
    HTS221_read_bytes(HTS221_T0_OUT_L | 0x80, t_out_data, 4);

    // Combine the bytes into the T0_OUT and T1_OUT values
    t0_out = (int16_t)(t_out_data[1] << 8 | t_out_data[0]);
    t1_out = (int16_t)(t_out_data[3] << 8 | t_out_data[2]);

    // Calculate the temperature calibration slope and intercept using the calibration values
    HTS221_TemperatureSlope = (t1_degC - t0_degC) / (8.0 * t1_out - t0_out);
    HTS221_TemperatureZero = (t0_degC / 8.0) - HTS221_TemperatureSlope * t0_out;
LL_mDelay(100);
}


void HTS221_get_temperature(float* temperature_out) {
    uint8_t t_out_buffer[2]; // Buffer to hold the two temperature bytes
    int16_t t_out;

    // Read both temperature registers at once
    HTS221_read_bytes(HTS221_TEMP_OUT_L | 0x80, t_out_buffer, 2);

    // Combine the two bytes into a single 16-bit value
    t_out = (int16_t)((uint16_t)t_out_buffer[1] << 8 | t_out_buffer[0]);

    // Calculate the temperature in Celsius
    *temperature_out = (t_out * -HTS221_TemperatureSlope) + HTS221_TemperatureZero;
}


float hts221_get_humidity() {

	float hum;

	//calibration registers
    uint8_t h0_x2 = hts221_read_byte(H0_rH_x2);
    uint8_t h1_x2 = hts221_read_byte(H1_rH_x2);

    float h0_rh = (float) h0_x2/2.0;
    float h1_rh = (float) h1_x2/2.0;

    uint16_t h0_t0_out = (hts221_read_byte(H0_OUT_1) ) | (hts221_read_byte(H0_OUT_2 )<<8);
    uint16_t h1_t0_out = (hts221_read_byte(H1_OUT_1) ) | (hts221_read_byte(H1_OUT_2)<<8);

    //output value
    uint16_t h_out = (hts221_read_byte(HTS221_HUM_OUT_L) ) | (hts221_read_byte(HTS221_HUM_OUT_H) <<8);

    //calculate humidity
    hum = (h0_rh + ((h1_rh-h0_rh) * ((float)h_out - (float)h0_t0_out)/((float)h1_t0_out-(float)h0_t0_out)));

    return hum;
}
