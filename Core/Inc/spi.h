/*
 * spi.h
 *
 *  Created on: Nov 26, 2024
 *      Author: prezentacie
 */

//#ifndef INC_SPI_H_
//#define INC_SPI_H_

#ifndef __SPI_H
#define __SPI_H


#include "stm32f3xx_hal.h"

extern SPI_HandleTypeDef hspi1;

// Deklarácie funkcií pre SPI
void MX_SPI1_Init(void);

#endif /* INC_SPI_H_ */
