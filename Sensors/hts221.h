/*
 * hts221.h
 *
 *  Created on: Oct 28, 2024
 *      Author: Piwkova
 */

#ifndef HTS221_H_
#define HTS221_H_

#include "stdint.h"
#include "i2c.h"

#define HTS221_I2C_ADDRESS 0xBE

#define HTS221_WHO_AM_I 0x0F
#define HTS221_CTRL_REG1 0x20

#define HTS221_WHO_AM_I_VALUE 0b10111100

#define HTS221_TEMP_OUT_L 0x2A
#define HTS221_TEMP_OUT_H 0x2B

#define HTS221_HUM_OUT_L 0x28
#define HTS221_HUM_OUT_H 0x29

#define T0_degC_x8 0x32
#define T1_degC_x8 0x33
#define MSB 0x35

#define T0_OUT_1 0x3C
#define T0_OUT_2 0x3D
#define T1_OUT_1 0x3E
#define T1_OUT_2 0x3F

#define H0_rH_x2 0x32
#define H1_rH_x2 0x33

#define H0_OUT_1 0x36
#define H0_OUT_2 0x37
#define H1_OUT_1 0x3A
#define H1_OUT_2 0x3B



//Same addresses, but kept different names
#define 	HTS221_TEMP_OUT_L			0x2A
#define 	HTS221_TEMP_OUT_H			0x2B
#define 	HTS221_H0_rH_x2				0x30
#define		HTS221_H1_rH_x2				0x31
#define 	HTS221_T0_degC_x8			0x32
#define		HTS221_T1_degC_x8			0x33
#define 	HTS221_T1_T0_MSB			0x34
#define 	HTS221_H0_T0_OUT_L			0x36
#define		HTS221_H0_T0_OUT_H			0x37
#define 	HTS221_H1_T0_OUT_L			0x3A
#define		HTS221_H1_T0_OUT_H			0x3B
#define 	HTS221_T0_OUT_L				0x3C
#define		HTS221_T0_OUT_H				0x3D
#define 	HTS221_T1_OUT_L				0x3E
#define		HTS221_T1_OUT_H				0x3F

void HTS221_read_bytes(uint8_t reg_addr, uint8_t* values, size_t length);
void HTS221_get_temperature_calibration(void);
void HTS221_get_temperature(float* temperature_out);
//over

//uint8_t HTS221_init();

void hts221_write_byte(uint8_t reg_addr, uint8_t value);
uint8_t hts221_read_byte(uint8_t reg_addr);

float hts221_get_temperature();
float hts221_get_humidity();

#endif /* HTS221_H_ */
