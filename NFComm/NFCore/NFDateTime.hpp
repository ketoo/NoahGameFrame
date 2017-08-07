// -------------------------------------------------------------------------
//    @FileName         :    NFDateTime.hpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-22
//    @Module           :    NFDateTime
//
// -------------------------------------------------------------------------

#ifndef NF_DATETIME_HPP
#define NF_DATETIME_HPP

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <time.h>
#include <string>
#include <map>
#include <vector>
#include "NFException.hpp"
#include "NFComm/NFPluginModule/NFPlatform.h"

//for timespan
#define FACTOR_SEC_TO_MILLI                     1000
#define FACTOR_MIN_TO_MILLI                60 * 1000
#define FACTOR_MIN_TO_SEC                         60
#define FACTOR_HOUR_TO_MILLI          60 * 60 * 1000
#define FACTOR_HOUR_TO_MIN                        60
#define FACTOR_DAY_TO_MILLI      24 * 60 * 60 * 1000
#define FACTOR_DAY_TO_HOUR                        24
///////////////////////////////
//for datetime
#define SINCE_YEAR               1900

// Summertime
#define SUMMERTIME_BEGIN_MONTH      3
#define SUMMERTIME_END_MONTH       10

// Min and Max values
#define MIN_MONTH                   1
#define MAX_MONTH                  12

#define MIN_DAY                     1
#define MAX_DAY                    30

#define MIN_WEEKDAY                 0
#define MAX_WEEKDAY                 7

#define MIN_HOUR                    0
#define MAX_HOUR                   24

#define MIN_MINUTE                  0
#define MAX_MINUTE                 60

#define MIN_SECOND                  0
#define MAX_SECOND                 60

#define MIN_MILLISECOND             0
#define MAX_MILLISECOND          1000

class NFTimeSpan
{
public:
	NFTimeSpan(int seconds)
	{
		Init(0, 0, 0, seconds, 0);
	}

	NFTimeSpan(int hours, int minutes, int seconds)
	{
		Init(0, hours, minutes, seconds, 0);
	}

	NFTimeSpan(int days, int hours, int minutes, int seconds)
	{
		Init(days, hours, minutes, seconds, 0);
	}

	NFTimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
	{
		Init(days, hours, minutes, seconds, milliseconds);
	}

	virtual ~NFTimeSpan()
	{

	}

	static NFTimeSpan FromMilliseconds(int milliseconds)
	{
		return NFTimeSpan(0, 0, 0, 0, milliseconds);
	}

	static NFTimeSpan FromSeconds(int seconds)
	{
		return NFTimeSpan(0, 0, 0, seconds, 0);
	}

	static NFTimeSpan FromMinutes(int minutes)
	{
		return NFTimeSpan(0, 0, minutes, 0, 0);
	}

	static NFTimeSpan FromHours(int hours)
	{
		return NFTimeSpan(0, hours, 0, 0, 0);
	}

	static NFTimeSpan FromDays(int days)
	{
		return NFTimeSpan(days, 0, 0, 0, 0);
	}

	int GetMilliseconds() const { return milliseconds; }
	int GetSeconds() const { return seconds; }
	int GetMinutes() const { return minutes; }
	int GetHours() const { return hours; }
	int GetDays() const { return days; }

	long long GetTotalMilliseconds() const
	{
		return milliseconds + seconds * FACTOR_SEC_TO_MILLI + minutes * FACTOR_MIN_TO_MILLI + hours * FACTOR_HOUR_TO_MILLI + days * FACTOR_DAY_TO_MILLI;
	}

	//updated : GetTotalXXXXs never return double
	long long GetTotalSeconds() const
	{
		return GetTotalMilliseconds() / FACTOR_SEC_TO_MILLI;
	}

	long long GetTotalMinutes() const
	{
		return GetTotalSeconds() / FACTOR_MIN_TO_SEC;
	}

	long long GetTotalHours() const
	{
		return GetTotalMinutes() / FACTOR_HOUR_TO_MIN;
	}

	long long GetTotalDays() const
	{
		return GetTotalHours() / FACTOR_DAY_TO_HOUR;
	}

	bool operator<(const NFTimeSpan &ts) const
	{
		return GetTotalMilliseconds() < ts.GetTotalMilliseconds();
	}

	bool operator>(const NFTimeSpan &ts) const
	{
		return GetTotalMilliseconds() > ts.GetTotalMilliseconds();
	}

	bool operator<=(const NFTimeSpan &ts) const
	{
		return GetTotalMilliseconds() <= ts.GetTotalMilliseconds();
	}

	bool operator>=(const NFTimeSpan &ts) const
	{
		return GetTotalMilliseconds() >= ts.GetTotalMilliseconds();
	}

	bool operator==(const NFTimeSpan &ts) const
	{
		return GetTotalMilliseconds() == ts.GetTotalMilliseconds();
	}

	void Init(int days, int hours, int minutes, int seconds, int milliseconds)
	{
		this->days = days;
		this->hours = hours;
		this->minutes = minutes;
		this->seconds = seconds;
		this->milliseconds = milliseconds;
	}

protected:

	int milliseconds;
	int seconds;
	int minutes;
	int hours;
	int days;
};

//////////////////////////////////////////////

class NFDateTime
{
public:

	enum Day
	{
		Monday,
		Thuesday,
		Wednesday,
		Thursday,
		Friday,
		Saturday,
		Sunday
	};

	enum Month
	{
		January,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};

	NFDateTime(int day, int month, int year)
	{
		Init(day, month, year, 0, 0, 0, 0);
	}

	NFDateTime(int day, int month, int year, int hour, int minute, int second)
	{
		Init(day, month, year, hour, minute, second, 0);
	}

	NFDateTime(int day, int month, int year, int hour, int minute, int second, int millisecond)
	{
		Init(day, month, year, hour, minute, second, millisecond);
	}

	NFDateTime(time_t timestamp)
	{
		SetWithTimestamp(timestamp);
	}

	~NFDateTime() 
	{
	}

	static NFDateTime Now()
	{
		NFDateTime dt;
		dt.SetNow();
		return dt;
	}

	void SetNow()
	{
		SetWithTimestamp(time(0));
	}
	
	int GetYear() const { return year; }
	int GetMonth() const { return month; }
	int GetDay() const { return day; }

	int GetHour() const { return hour; }
	int GetMinute() const { return minute; }
	int GetSecond() const { return second; }
	int GetMillisecond() const { return millisecond; }


	void SetWithTimestamp(time_t timestamp)
	{
		struct tm time = *localtime(&timestamp);
		Init(time.tm_mday, time.tm_mon + 1, time.tm_year + SINCE_YEAR, time.tm_hour, time.tm_min, time.tm_sec, 0);
	}

	time_t GetTimestamp() const
	{
		time_t rawtime;
		struct tm *time = localtime(&rawtime);
		time->tm_year = year - SINCE_YEAR;
		time->tm_mon = month - 1;
		time->tm_mday = day;
		time->tm_hour = hour;
		time->tm_min = minute;
		time->tm_sec = second;
		return mktime(time);
	}

	struct tm GetTMStruct() const
	{
		time_t rawtime;
		struct tm *time = localtime(&rawtime);
		time->tm_year = year - SINCE_YEAR;
		time->tm_mon = month - 1;
		time->tm_mday = day;
		time->tm_hour = hour;
		time->tm_min = minute;
		time->tm_sec = second;
		mktime(time);
		return *time;
	}

	void Add(const NFTimeSpan *ts)
	{
		AddMilliseconds(ts->GetTotalMilliseconds());
	}

	void AddYears(int years)
	{
		year += years;
	}

	void AddMonths(int months)
	{
		AddYears(months / MAX_MONTH);
		month += months % MAX_MONTH;
	}

	void AddDays(int days)
	{
		AddMonths(days / MAX_DAY);
		day += days % MAX_DAY;
	}

	void AddHours(int hours)
	{
		AddDays(hours / MAX_HOUR);
		hour += hours % MAX_HOUR;
	}

	void AddMinutes(int minutes)
	{
		AddHours(int(minutes / MAX_MINUTE));
		minute += minutes % MAX_MINUTE;
	}

	void AddSeconds(long long seconds)
	{
		AddMinutes(int(seconds / MAX_SECOND));
		second += int(seconds % MAX_SECOND);
	}

	void AddMilliseconds(long long milliseconds)
	{
		AddSeconds(milliseconds / MAX_MILLISECOND);
		millisecond += milliseconds % MAX_MILLISECOND;
	}

	static bool IsYearLeapYear(int year)
	{
		return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
	}

	bool IsSummertime() { return IsYearLeapYear(year); }
	bool IsLeapYear() { return IsDateSummertime(day, month); }

	static bool IsDateSummertime(int day, int month)
	{
		// FIXME: include day in calculation
		if (month >= SUMMERTIME_BEGIN_MONTH && month <= SUMMERTIME_END_MONTH)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	std::string GetNameOfDay(int day)
	{
		if (IsValidWeekday(day))
		{
			return dayNames[day - 1];
		}
		else
		{
			throw NFException("Day %d is not in valid weekday range ( %d - %d )", day, MIN_WEEKDAY, MAX_WEEKDAY);
		}
	}

	std::string GetNameOfMonth(int month)
	{
		if (IsValidMonth(month))
		{
			return monthNames[month - 1];
		}
		else
		{
			throw NFException("Month %d is not in valid range ( %d - %d )", month, MIN_MONTH, MAX_MONTH);
		}
	}

	Day GetDayOfWeek()
	{
		return static_cast<NFDateTime::Day>(GetTMStruct().tm_wday);
	}

	int GetDayOfYear()
	{
		return GetTMStruct().tm_yday;
	}

	int GetDaysOfMonth(int month)
	{
		return IsValidMonth(month) ? daysOfMonth[month] : -1;
	}

	bool operator<(const NFDateTime &dt) const
	{
		return GetTimestamp() < dt.GetTimestamp();
	}

	bool operator>(const NFDateTime &dt) const
	{
		return GetTimestamp() > dt.GetTimestamp();
	}

	bool operator<=(const NFDateTime &dt) const
	{
		return GetTimestamp() <= dt.GetTimestamp();
	}

	bool operator>=(const NFDateTime &dt) const
	{
		return GetTimestamp() >= dt.GetTimestamp();
	}

	bool operator==(const NFDateTime &dt) const
	{
		return GetTimestamp() == dt.GetTimestamp();
	}

	NFDateTime operator+(const NFTimeSpan &ts)
	{
		NFDateTime *tmp = this;
		tmp->Add(&ts);
		return *tmp;
	}

	void operator+=(const NFTimeSpan &ts)
	{
		Add(&ts);
	}


	std::string GetAsString()
	{
		return GetShortDateString() + std::string(" - ") + GetShortTimeString();
	}

	std::string GetShortTimeString()
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << std::setfill('0') << std::setw(2) << hour << ":" << std::setw(2) << minute << ":" << std::setw(2) << second;
		return ss.str();
	}

	std::string GetLongTimeString()
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << std::setfill('0') << std::setw(2) << hour << ":" << std::setw(2) << minute << ":" << std::setw(2) << second << ":" << std::setw(2) << millisecond;
		return ss.str();
	}

	std::string GetShortDateString()
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << std::setfill('0') << std::setw(2) << day << "." << std::setw(2) << month << "." << year;
		return ss.str();
	}

	std::string GetLongDateString()
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << GetNameOfDay(GetDayOfWeek()) << ", " << GetNameOfMonth(month) << " " << day << ", " << year;
		return ss.str();
	}

protected:
	NFDateTime()
	{
		this->day = 0;
		this->month = 0;
		this->year = 0;

		this->hour = hour;
		this->minute = minute;
		this->second = second;
		this->millisecond = millisecond;

		InitMonths();
		InitMonthNames();
		InitDayNames();
	}

	void Init(int day, int month, int year, int hour, int minute, int second, int millisecond)
	{
		this->year = year;

		InitMonths();
		InitMonthNames();
		InitDayNames();

		if (!IsValidMonth(month))
		{
			throw NFException("Month %d is not in range", month);
		}

		if (!IsValidDayOfMonth(day, month))
		{
			throw NFException("Day %d is not in month %d's range", day, month);
		}

		if (!IsValidHour(hour))
		{
			throw NFException("Hour %d is not in valid range ( %d - %d )", hour, MIN_HOUR, MAX_HOUR);
		}

		if (!IsValidMinute(minute))
		{
			throw NFException("Minute %d is not in valid range ( %d - %d )", minute, MIN_MINUTE, MAX_MINUTE);
		}

		if (!IsValidSecond(second))
		{
			throw NFException("Second %d is not in valid range ( %d - %d )", second, MIN_SECOND, MAX_SECOND);
		}

		if (!IsValidMillisecond(millisecond))
		{
			throw NFException("Millisecond %d is not in valid range ( %d - %d )", millisecond, MIN_MILLISECOND, MAX_MILLISECOND);
		}

		this->day = day;
		this->month = month;

		this->hour = hour;
		this->minute = minute;
		this->second = second;
		this->millisecond = millisecond;
	}

	void InitMonths()
	{
		// Perhaps an algorithm would be more efficient
		daysOfMonth[Month::January] = 31;
		daysOfMonth[Month::February] = IsLeapYear() ? 29 : 28; // In a leapyear 29 else 28
		daysOfMonth[Month::March] = 31;
		daysOfMonth[Month::April] = 30;
		daysOfMonth[Month::May] = 31;
		daysOfMonth[Month::June] = 30;
		daysOfMonth[Month::July] = 31;
		daysOfMonth[Month::August] = 31;
		daysOfMonth[Month::September] = 30;
		daysOfMonth[Month::October] = 31;
		daysOfMonth[Month::November] = 30;
		daysOfMonth[Month::December] = 31;
	}

	void InitMonthNames()
	{
		monthNames[Month::January] = "January";
		monthNames[Month::February] = "February";
		monthNames[Month::March] = "March";
		monthNames[Month::April] = "April";
		monthNames[Month::May] = "May";
		monthNames[Month::June] = "June";
		monthNames[Month::July] = "July";
		monthNames[Month::August] = "August";
		monthNames[Month::September] = "September";
		monthNames[Month::October] = "October";
		monthNames[Month::November] = "November";
		monthNames[Month::December] = "December";
	}

	void InitDayNames()
	{
		dayNames[Day::Monday] = "Monday";
		dayNames[Day::Thuesday] = "Thuesday";
		dayNames[Day::Wednesday] = "Wednesday";
		dayNames[Day::Thursday] = "Thursday";
		dayNames[Day::Friday] = "Friday";
		dayNames[Day::Saturday] = "Saturday";
		dayNames[Day::Sunday] = "Sunday";
	}

	bool IsValidWeekday(int day)
	{
		return day >= MIN_WEEKDAY && day <= MAX_WEEKDAY;
	}

	bool IsValidDayOfMonth(int day, int month)
	{
		if (IsValidMonth(month))
		{
			return day >= 1 && day <= GetDaysOfMonth(month);
		}
		else
		{
			return false;
		}
	}

	bool IsValidMonth(int month)
	{
		return month >= MIN_MONTH && month <= MAX_MONTH;
	}

	bool IsValidYear(int year)
	{
		return year >= 0;
	}

	bool IsValidHour(int hour)
	{
		return hour >= MIN_HOUR && hour <= MAX_HOUR;
	}

	bool IsValidMinute(int minute)
	{
		return minute >= MIN_MINUTE && minute <= MAX_MINUTE;
	}

	bool IsValidSecond(int second)
	{
		return second >= MIN_SECOND && second <= MAX_SECOND;
	}

	bool IsValidMillisecond(int millisecond)
	{
		return millisecond >= MIN_MILLISECOND && millisecond <= MAX_MILLISECOND;
	}
	
protected:
	int daysOfMonth[MAX_MONTH];
	std::string monthNames[MAX_MONTH];
	std::string dayNames[MAX_DAY];

	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int millisecond;

	bool summertime;
	bool leapyear;
};

#endif



