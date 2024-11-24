/*
 * lps25hb.c
 *
 *  Created on: Oct 28, 2024
 *      Author: Piwkova
 */

#include "lps25hb.h"

uint8_t adress = LPS25HB_I2C_ADDRESS_0;

uint8_t LPS25HB_init(){

	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t val = lps25hb_read_byte(LPS25HB_WHO_AM_I);

	if (val == WHO_AM_I_VALUE_LPS25HB) {
		//setup register
		uint8_t ctrl_reg1 = 0b11000000;
		lps25hb_write_byte(LPS25HB_CTRL_REG1, ctrl_reg1);
		status = 1;
	} else {
		// second address
		adress = LPS25HB_I2C_ADDRESS_1;
		val = lps25hb_read_byte(LPS25HB_WHO_AM_I);
		if (val == WHO_AM_I_VALUE_LPS25HB) {
			//setup register
			uint8_t ctrl_reg1 = 0b11000000;
			lps25hb_write_byte(LPS25HB_CTRL_REG1, ctrl_reg1);
			status = 1;
		}
		else {
			status = 0;
		}
	}
	return status;
}

uint8_t lps25hb_read_byte(uint8_t reg_addr) {
	uint8_t val = 0;
	i2c_master_read(&val, 1, reg_addr, adress);
	return val;
}

void lps25hb_write_byte(uint8_t reg_addr, uint8_t val) {
	i2c_master_write(val, reg_addr, adress);
}

//read pressure
float lps25hb_get_pressure() {

	float press;

	uint8_t xl = lps25hb_read_byte(LPS25HB_PRESS_OUT_XL);
	uint8_t l = lps25hb_read_byte(LPS25HB_PRESS_OUT_L);
	uint8_t h = lps25hb_read_byte(LPS25HB_PRESS_OUT_H);

	//calculate pressure
	press = ((float) ( xl | (l << 8) | (h << 16))) / 4096.0;

	return press;
}
