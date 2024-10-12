Read Date and Time of STM32-RTC and convert Gregorian Date to Persian Date or Persian to Gregorian.

Sample:
--------------------------------------------------------
#include "Calendar.h"
extern RTC_HandleTypeDef hrtc;

//get Date and Time of RTC:
  Calendar::Date date= Calendar::RTCToPersian(&hrtc);

//Convert Gregorian Date to Persian Date:
  Calendar::Date gdate = { 2024, 10, 1 };                      		// gdate.Year = 2024; gdate.Month = 10; gdate.Day = 1;
  Calendar::Date pdate = Calendar::GregorianToPersian(gdate);   	// pdate.Year = 1403; pdate.Month = 7; pdate.Day = 10;

//Convert Persian Date to Gregorian Date:
  Calendar::Date Pdate = { 1403, 7, 10 };                       	// Pdate.Year = 1403; Pdate.Month = 7; Pdate.Day = 10;
  Calendar::Date Gdate = Calendar::PersianToGregorian(Pdate);     // Gdate.Year = 2024; Gdate.Month = 10; Gdate.Day = 1;

