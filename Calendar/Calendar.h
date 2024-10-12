#pragma once
#include "main.h"

extern RTC_HandleTypeDef hrtc;

#include <stdint.h>

class Calendar {
private:
    Calendar();
    
public:
    struct Date {
        int32_t Year=2000;
        int32_t Month=1;
        int32_t Day=1;
        int32_t hour=00;
        int32_t minute=00;
        int32_t second=00;
    };

    // تبدیل تاریخ RTC به تاریخ شمسی
    static Date RTCToPersian(RTC_HandleTypeDef *hrtc) {
        RTC_DateTypeDef pDate;
        RTC_TimeTypeDef pTime;
        HAL_RTC_GetDate(hrtc, &pDate, RTC_FORMAT_BCD);
        HAL_RTC_GetTime(hrtc, &pTime, RTC_FORMAT_BCD);

        int32_t year = BCDToDecimal(pDate.Year) + 2000; // Assuming year is in 00-99 format
        int32_t month = BCDToDecimal(pDate.Month); // تبدیل ماه از BCD به ده‌دهی
        int32_t day = BCDToDecimal(pDate.Date); // تبدیل روز از BCD به ده‌دهی
        
        int32_t hour = BCDToDecimal(pTime.Hours); 
        int32_t minute = BCDToDecimal(pTime.Minutes); 
        int32_t second = BCDToDecimal(pTime.Seconds); 

        Date date = { year, month, day, hour, minute, second};
        
        return GregorianToPersian(date);
    }
    
    static Date RTCToGregorian(RTC_HandleTypeDef *hrtc) {
        RTC_DateTypeDef pDate;
        RTC_TimeTypeDef pTime;
        HAL_RTC_GetDate(hrtc, &pDate, RTC_FORMAT_BCD);
        HAL_RTC_GetTime(hrtc, &pTime, RTC_FORMAT_BCD);

        int32_t year = BCDToDecimal(pDate.Year) + 2000; // Assuming year is in 00-99 format
        int32_t month = BCDToDecimal(pDate.Month); // تبدیل ماه از BCD به ده‌دهی
        int32_t day = BCDToDecimal(pDate.Date); // تبدیل روز از BCD به ده‌دهی
        
        int32_t hour = BCDToDecimal(pTime.Hours); 
        int32_t minute = BCDToDecimal(pTime.Minutes); 
        int32_t second = BCDToDecimal(pTime.Seconds); 

        Date date = { year, month, day, hour, minute, second };
        return date;
    }


    static Date GregorianToPersian(Date GregorianDate) {
        Date PDate;
        long  out;
        PDate.Year = (GregorianDate.Month > 2) ? (GregorianDate.Year + 1) : GregorianDate.Year;
        {
            long g_d_m[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
            out = 355666 + (365 * GregorianDate.Year) + ((int)((PDate.Year + 3) / 4)) - ((int)((PDate.Year + 99) / 100)) + ((int)((PDate.Year + 399) / 400)) + GregorianDate.Day + g_d_m[GregorianDate.Month - 1];
        }
        PDate.Year = -1595 + (33 * ((int)(out / 12053)));
        out %= 12053;
        PDate.Year += 4 * ((int)(out / 1461));
        out %= 1461;
        if (out > 365) {
            PDate.Year += (int)((out - 1) / 365);
            out = (out - 1) % 365;
        }
        if (out < 186) {
            PDate.Month = 1 + (int)(out / 31);
            PDate.Day = 1 + (out % 31);
        }
        else {
            PDate.Month = 7 + (int)((out - 186) / 30);
            PDate.Day = 1 + ((out - 186) % 30);
        }
        
        PDate.hour =  GregorianDate.hour;
        PDate.minute =  GregorianDate.minute;
        PDate.second =  GregorianDate.second;
        return PDate;
    }

    static Date PersianToGregorian(Date PersianDate) {
        Date GDate;
        long  out;
        PersianDate.Year += 1595;
        GDate.Day = -355668 + (365 * PersianDate.Year) + (((int)(PersianDate.Year / 33)) * 8) + ((int)(((PersianDate.Year % 33) + 3) / 4)) + PersianDate.Day + ((PersianDate.Month < 7) ? (PersianDate.Month - 1) * 31 : ((PersianDate.Month - 7) * 30) + 186);
        GDate.Year = 400 * ((int)(GDate.Day / 146097));
        GDate.Day %= 146097;
        if (GDate.Day > 36524) {
            GDate.Year += 100 * ((int)(--GDate.Day / 36524));
            GDate.Day %= 36524;
            if (GDate.Day >= 365) GDate.Day++;
        }
        GDate.Year += 4 * ((int)(GDate.Day / 1461));
        GDate.Day %= 1461;
        if (GDate.Day > 365) {
            GDate.Year += (int)((GDate.Day - 1) / 365);
            GDate.Day = (GDate.Day - 1) % 365;
        }
        long sal_a[13] = { 0, 31, ((GDate.Year % 4 == 0 && GDate.Year % 100 != 0) || (GDate.Year % 400 == 0)) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        for (GDate.Day++, GDate.Month = 0; GDate.Month < 13 && GDate.Day > sal_a[GDate.Month]; GDate.Month++) GDate.Day -= sal_a[GDate.Month];
        
        GDate.hour =  PersianDate.hour;
        GDate.minute =  PersianDate.minute;
        GDate.second =  PersianDate.second;
        return GDate;
    }


private:
    // تبدیل مقدار BCD به ده‌دهی
    static int32_t BCDToDecimal(int32_t bcd) {
        return ((bcd >> 4) * 10) + (bcd & 0x0F);
    }
};
