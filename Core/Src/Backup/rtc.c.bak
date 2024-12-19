
#include "rtc.h"
#include "ili9163.h"
#include "stm32f3xx_ll_rcc.h"
#include "stm32f3xx_ll_rtc.h"
#include "stm32f3xx_ll_bus.h"
#include "stm32f3xx_ll_pwr.h"
#include "stm32f3xx_ll_exti.h"
#include "stm32f3xx_ll_system.h"
#include "stm32f3xx_hal_rtc.h"
#include "Minimum_font.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

RTC_TimeTypeDef sTime;

// Inicializácia RTC manuálne, priamym zapisovaním do registrov
void RTC_Init(void)
{
    // Povolenie napájania prístupov k záložnej oblasti
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
    LL_PWR_EnableBkUpAccess();

    // Nastavenie zdroja hodin pre RTC na LSI (Low-Speed Internal)
    LL_RCC_LSI_Enable();
    while (LL_RCC_LSI_IsReady() != 1)
    {
        // Čakanie na stabilizáciu LSI
    }

    // Nastavenie zdroja hodin pre RTC
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_LSI);
    LL_RCC_EnableRTC();

    // Konfigurácia RTC (24-hodinový formát, prednastavenia)
    // Zakázať ochranu proti zápisu
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    // Nastaviť formát hodín na 24-hodinový
    RTC->CR &= ~(RTC_CR_FMT);

    // Nastaviť asynchrónny a synchrónny predelovač
    RTC->PRER = (127 << RTC_PRER_PREDIV_A_Pos) | (255 << RTC_PRER_PREDIV_S_Pos);

    // Povoliť ochranu proti zápisu
    RTC->WPR = 0xFF;
}
uint8_t bcd_to_bin(uint8_t bcd)
{
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

#include <stdint.h> // For standard integer types

// Manually define struct timeval if <sys/time.h> is not available
struct timeval {
    long tv_sec;  // seconds
    long tv_usec; // microseconds (not used here, can be set to 0)
};

// Function to get the current time from RTC
int _gettimeofday(struct timeval *tv, void *tzvp) {
    if (tv) {
        // Disable write protection to access RTC registers
        LL_RTC_DisableWriteProtection(RTC);

        // Read time and date from the RTC registers
        uint32_t time_reg __attribute__((unused))= LL_RTC_TIME_Get(RTC);
        uint32_t date_reg __attribute__((unused))= LL_RTC_DATE_Get(RTC);

        // Extract time components from RTC register
        uint8_t hours = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetHour(RTC));
        uint8_t minutes = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetMinute(RTC));
        uint8_t seconds = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_TIME_GetSecond(RTC));

        // Extract date components from RTC register
        uint8_t year = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetYear(RTC));
        uint8_t month = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetMonth(RTC));
        uint8_t day = __LL_RTC_CONVERT_BCD2BIN(LL_RTC_DATE_GetDay(RTC));

        // Convert to epoch time (basic conversion without leap year consideration)
        tv->tv_sec = (year + 2000 - 1970) * 365 * 24 * 3600 // Basic year calculation
                   + (month - 1) * 30 * 24 * 3600           // Basic month calculation (rough estimate)
                   + (day - 1) * 24 * 3600                  // Day calculation
                   + hours * 3600
                   + minutes * 60
                   + seconds;

        tv->tv_usec = 0;

        // Re-enable write protection
        LL_RTC_EnableWriteProtection(RTC);

        return 0; // Success
    }
    return -1; // Failure
}


void get_current_time(void)
{
    // Zakázať ochranu proti zápisu
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;

    // Prečítať hodnoty hodín, minút a sekúnd
    uint32_t time = RTC->TR;

    // Povoliť ochranu proti zápisu
    RTC->WPR = 0xFF;

    // Dekódovanie hodnoty času z formátu BCD
    sTime.Hours = ((time & (RTC_TR_HT | RTC_TR_HU)) >> 16);
    sTime.Minutes = ((time & (RTC_TR_MNT | RTC_TR_MNU)) >> 8);
    sTime.Seconds = (time & (RTC_TR_ST | RTC_TR_SU));
}






