/*

Author: Chongliu Jia

*/

#ifndef WEASEL_CORE_DATE_H
#define WEASEL_CORE_DATE_H

#include "weasel/core/Copyable.h"
#include "weasel/core/Types.h"

struct tm; // see "explicit Date(const struct tm&)"

namespace weasel
{
	class Date : public weasel::copyable
	{
		public:
			struct MonthDayYear 
			{
				int month; //month[1-12]
				int day;   //day[1-31]
				int year;  //year
			};

			static const int DayinWeek = 7;

			Date() : DayNumber_(0) 
			{
			}

			// month, day, year ==> mm-dd-yyyy
			Date(int month, int day, int year);

			explicit Date(int DayNumber) : DayNumber(DayNumber_)
			{
			}

			// constructs a date from struct tm
			explicit Date(const struct tm&);

			void swap(Date& that)
			{
				std::swap(DayNumber_, that.DayNumber_);
			}

			bool valid() const 
			{
				return DayNumber_ > 0;
			}
			/*
			Converts to mm-dd-yyyy format
			*/

			string toIsoString() const;

			struct MonthDayYear monthDayYear() const;

			int month() const
			{
				return monthDayYear().month;
			}

			int day() const
			{
				return monthDayYear().day;
			}

			int year() const
			{
				return monthDayYear().year;
			}

			int WeekDay() const 
			{
				return (DayNumber_ + 1) % DayinWeek;
			}

			int DayNumber() const 
			{
				return DayNumber_;
			}

		private:
			int DayNumber_;

	};

	inline bool operator < (Date x, Date y) 
	{
		return x.DayNumber() < y.DayNumber();
	}

	inline bool operator == (Date x, Date y)
	{
		return x.DayNumber() == y.DayNumber();
	}

}

#endif
