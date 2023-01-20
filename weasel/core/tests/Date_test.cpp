#include "../Date.h"

#include <assert.h>
#include <stdio.h>
#include <time.h>

using weasel::Date;

const int kMonthofYear = 12;

int IsLeapYear(int year);
int DayofMonth(int year, int month);

void PassByConstReference(const Date& x);
void PassByValue(Date x);

int main()
{
	time_t now = time(NULL);

	struct tm t1 = *gmtime(&now);
	struct tm t2 = *localtime(&now);

	Date someDay(1997, 10, 1);
	printf("%s\n", someDay.toIsoString().c_str());

	PassByValue(someDay);
	PassByConstReference(someDay);

	Date toDay(t1);
	printf("%s\n", toDay.toIsoString().c_str());

	Date toDay2(t2);
	printf("%s\n", toDay2.toIsoString().c_str());

	int julianDayNumber = 2415021;
	int weekDay = 1;

	for (int year = 1900; year < 2900; ++year) {
		assert(Date(year, 3, 1).DayNumber() - Date(year, 2, 29).DayNumber() == IsLeapYear(year));

		for (int month = 1; month <= kMonthofYear; ++month) {
			for (int day = 1; day <= DayofMonth(year, month); ++day) {
				Date d(year, month, day);

				assert(year == d.year());
				assert(month == d.month());
				assert(day == d.day());
				assert(weekDay == d.WeekDay());
				assert(julianDayNumber == d.DayNumber());

				Date d2(julianDayNumber);

				assert(year == d2.year());
				assert(month == d2.month());
				assert(day == d2.day());
				assert(weekDay == d2.WeekDay());
				assert(julianDayNumber == d2.DayNumber());

				++julianDayNumber;
				weekDay = (weekDay + 1) % 7;
			}
		}
	}

	printf("done !");

	return 0;
}
	

int IsLeapYear(int year)
{
	if (year % 400 == 0) return 1;
	else if (year % 100 == 0) return 0;
	else if (year % 4 == 0) return 1;
	else return 0;
}

int DayofMonth(int year, int month) 
{
	static int days[2][kMonthofYear + 1] = {
		{0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
		{0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
	};

	return days[IsLeapYear(year)][month];
}

void PassByConstReference(const Date& x)
{
	printf("%s\n", x.toIsoString().c_str());
}

void PassByValue(Date x)
{
	printf("%s\n", x.toIsoString().c_str());
}

