/*

Author: Chongliu Jia

*/

#ifndef WEASEL_CORE_DATE_H
#define WEASEL_CORE_DATE_H

#include "Copyable.h"
#include "Types.h"

struct tm; // see "explicit Date(const struct tm&)"

namespace weasel
{
	class Date : public weasel::copyable
	{
		public:
			struct YearMonthDay 
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
			Date(int year, int month, int day);

			explicit Date(int DayNumber) : DayNumber_(DayNumber)
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

			struct YearMonthDay yearMonthDay() const;

			int month() const
			{
				return yearMonthDay().month;
			}

			int day() const
			{
				return yearMonthDay().day;
			}

			int year() const
			{
				return yearMonthDay().year;
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
