/*
 * rtc.h
 *
 *  Created on: Dec 3, 2024
 *      Author: leneros
 */

#ifndef RTC_H_
#define RTC_H_

#include "ili9163.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_pwr.h"
#include "stm32f3xx_ll_exti.h"
#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_hal_rtc.h"
#include "Minimum_font.h"
#include <stdio.h>

typedef struct  {
    long tv_sec;  // seconds
    long tv_usec; // microseconds (not used here, can be set to 0)
}timeval;

extern RTC_TimeTypeDef sTime;

void RTC_Init(void);
void get_current_time(void);
uint8_t bcd_to_bin(uint8_t bcd);
void display_time(void);
void display_large_time(void);






#endif /* RTC_H_ */
