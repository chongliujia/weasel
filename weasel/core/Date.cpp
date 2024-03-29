#include "Date.h"

#include <stdio.h>
#include <time.h>

namespace weasel
{
	namespace detail 
	{
		char require_32_bit_integer_at_least[sizeof(int) >= sizeof(int32_t) ? 1 : -1];

		int GetJulianDayNumber(int year, int month, int day) 
		{
			int a = (14 - month) / 12;
			int y = year + 4800 - a;
			int m = month + 12 * a - 3;

			return day + (153 * m + 2) / 5 + y * 365 + y / 4 - y/100 + y/400 - 32045;
		}

		struct Date::YearMonthDay GetYearMonthDay(int julianDayNumber)
		{
			int a = julianDayNumber + 32044;
			int b = (4 * a + 3) / 146097;
			int c = a - ((b * 146097) / 4);
			int d = (4 * c + 3) / 1461;
			int e = c - ((1461 * d) / 4);
			int m = (5 * e + 2) / 153;

			Date::YearMonthDay ymd;
			ymd.day   = e - ((153 * m + 2) / 5) + 1;
			ymd.month = m + 3 - 12 * (m / 10);
			ymd.year  = b * 100 + d - 4800 + (m / 10);

			return ymd;
		}
	}
	
}

using namespace weasel;
using namespace weasel::detail;

Date::Date(int y, int m, int d) 
	: DayNumber_(GetJulianDayNumber(y, m, d))
{
}

Date::Date(const struct tm& t)
	: DayNumber_(GetJulianDayNumber(t.tm_year+1900, t.tm_mon+1, t.tm_mday))
{
}

string Date::toIsoString() const
{
	char buf[32];
	YearMonthDay ymd(yearMonthDay());
	snprintf(buf, sizeof(buf), "%4d-%02d-%02d", ymd.year, ymd.month, ymd.day);
	return buf;
}

Date::YearMonthDay Date::yearMonthDay() const
{
	return GetYearMonthDay(DayNumber_);
}

