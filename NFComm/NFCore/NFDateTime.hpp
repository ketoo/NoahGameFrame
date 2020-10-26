/*
This file is part of:
NoahFrame
https://github.com/ketoo/NoahGameFrame

Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

File creator: lvsheng.huang

NoahFrame is open-source software and you can redistribute it and/or modify
it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement; besides, anyone who use this file/software
must include this copyright announcement.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#pragma once
#ifndef NF_DATETIME_HPP
#define NF_DATETIME_HPP

#include <sstream>
#include <iomanip>
#include <string>
#include <chrono>

#define STR_CASE(_Enum, _Str) case _Enum::_Str : return # _Str
#define MONTH_CASE(_Month) STR_CASE(::Month, _Month)
#define DAY_CASE(_Day) STR_CASE(::DayOfWeek, _Day)

using namespace std::chrono;

enum class DayOfWeek
{
    Sunday = 0,
    Monday = 1,
    Tuesday = 2,
    Wednesday = 3,
    Thursday = 4,
    Friday = 5,
    Saturday = 6,
};

enum class Month
{
    January = 1,
    February = 2,
    March = 3,
    April = 4,
    May = 5,
    June = 6,
    July = 7,
    August = 8,
    September = 9,
    October = 10,
    November = 11,
    December = 12,
};

class NFDateTime;

class NFTimeSpan
{
public:
	static constexpr int64_t SecondPerMinute = 60;
	static constexpr int64_t SecondPerHour = SecondPerMinute * 60;
	static constexpr int64_t SecondPerDay = SecondPerHour * 24;
	static constexpr int64_t SecondPerWeek = SecondPerDay * 7;

	NFTimeSpan( int seconds) :
			totalSecond(seconds)
	{}

	NFTimeSpan(int hours, int minutes, int seconds) :
		NFTimeSpan(0, hours, minutes, seconds)
	{}

	NFTimeSpan(int days, int hours, int minutes, int seconds) :
			totalSecond(days * SecondPerDay + hours * SecondPerHour + minutes * SecondPerMinute + seconds)
	{}

	inline int TotalSecond() const
	{
		return totalSecond;
	}

	inline int Seconds() const
	{
		return totalSecond - totalSecond / SecondPerMinute;
	}

	inline int Minutes() const
	{
		return (totalSecond - totalSecond / SecondPerHour) / SecondPerMinute;
	}

	inline int Hours() const
	{
		return (totalSecond - totalSecond / SecondPerDay) / SecondPerHour;
	}

	inline int Days() const
	{
		return totalSecond / SecondPerDay;
	}

	inline void AddSeconds(int value)
	{
		totalSecond += value;
	}

	inline void AddMinutes(int value)
	{
		totalSecond += value * SecondPerMinute;
	}

	inline void AddHours(int value)
	{
		totalSecond += value * SecondPerHour;
	}

	inline void AddDays(int value)
	{
		totalSecond += value * SecondPerDay;
	}

	static bool Equals(const NFTimeSpan& t1, const NFTimeSpan& t2)
	{
		return t1.totalSecond == t2.totalSecond;
	}

	inline bool Equals(const NFTimeSpan& value) const
	{
		return totalSecond == value.totalSecond;
	}

	inline NFTimeSpan Subtract(const NFTimeSpan& ts) const
	{
		return totalSecond - ts.totalSecond;
	}

	inline std::string ToString() const
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << std::setfill('0') << Days() << "." << std::setw(2) << Hours() << ":" << std::setw(2) << Minutes() << ":" << std::setw(2) << Seconds();
		return ss.str();
	}

	inline void operator + (const NFTimeSpan& t)
	{
		totalSecond += t.totalSecond;
	}

	inline void operator - (const NFTimeSpan& t)
	{
		totalSecond -= t.totalSecond;
	}

	inline bool operator ==(const NFTimeSpan& t)
	{
		return totalSecond == t.totalSecond;
	}

	inline bool operator !=(const NFTimeSpan& t)
	{
		return totalSecond != t.totalSecond;
	}

	inline bool operator <(const NFTimeSpan& t)
	{
		return totalSecond < t.totalSecond;
	}

	inline bool operator >(const NFTimeSpan& t)
	{
		return totalSecond > t.totalSecond;
	}

	inline bool operator <=(const NFTimeSpan& t)
	{
		return totalSecond <= t.totalSecond;
	}

	inline bool operator >=(const NFTimeSpan& t)
	{
		return totalSecond >= t.totalSecond;
	}

protected:
	int64_t  totalSecond;
    friend class NFDateTime;
};


class NFDateTime
{
public:

	//seconds, from 1970-01-01
	//std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1000
	NFDateTime(const int64_t seconds) :
			_totalSecond(seconds)
	{
		UpdateTM();
	}

	//yyyy-MM-dd hh:mm:ss
	NFDateTime(const std::string& data)
	{
		try
		{
			std::stringstream ss(data);
			ss >> std::get_time(&_tm, "%Y-%m-%d %H:%M:%S");
			_totalSecond = mktime(&_tm);
		}
		catch (...)
		{

		}
	}

	NFDateTime(int year, int month, int day) :
		NFDateTime(year, month, day, 0, 0, 0)
	{}


	NFDateTime(int year, int month, int day, int hour, int minute, int second) :
			_tm{ second, minute, hour, day, month, year -1900, 0, 0, -1}
	{
		_totalSecond = mktime(&_tm);
	}

	static NFDateTime Now()
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

	inline int Month() const
	{
		return _tm.tm_mon + 1;
	}

	inline int Minute() const
	{
		return _tm.tm_min;
	}

	inline int Millisecond() const
	{
		return _totalSecond;
	}

	inline int Hour() const
	{
		return _tm.tm_hour;
	}

	inline int DayOfYear() const
	{
		return _tm.tm_yday;
	}

	inline ::DayOfWeek DayOfWeek() const
	{
		return static_cast<::DayOfWeek>(_tm.tm_wday);
	}

	inline int WeekOfYear() const
	{
		return _tm.tm_yday / 7;
	}

	inline int Day() const
	{
		return _tm.tm_mday;
	}

	inline int Second() const
	{
		return _tm.tm_sec;
	}

	inline NFTimeSpan TimeOfDay() const
	{
		return NFTimeSpan(0, _tm.tm_hour, _tm.tm_min, _tm.tm_sec);
	}

	inline int Year() const
	{
		return _tm.tm_year + 1900;
	}

	static int DaysInMonth(int year, int month)
	{
		switch (static_cast<::Month>(month))
		{
		case ::Month::January:
		case ::Month::March:
		case ::Month::May:
		case ::Month::July:
		case ::Month::August:
		case ::Month::October:
		case ::Month::December:
			return 31;

		case ::Month::April:
		case ::Month::June:
		case ::Month::September:
		case ::Month::November:
			return 30;

		case ::Month::February:
			return IsLeapYear(year) ? 29 : 28;

		default:
			break;
		}

		return -1;
	}

	static bool IsLeapYear(int year)
	{
		return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
	}

	inline void Add(const NFTimeSpan& value)
	{
		_totalSecond += value.totalSecond;
		UpdateTM();
	}

	inline void AddDays(const int value)
	{
		_totalSecond += value * NFTimeSpan::SecondPerDay;
		UpdateTM();
	}

	inline void AddHours(const int value)
	{
		_totalSecond += value * NFTimeSpan::SecondPerHour;
		UpdateTM();
	}

	inline void AddMinutes(const int value)
	{
		_totalSecond += value * NFTimeSpan::SecondPerMinute;
		UpdateTM();
	}

	inline void AddSeconds(const int value)
	{
		_totalSecond += value;
		UpdateTM();
	}

	inline NFDateTime AddMonths(const int value) const
	{
		auto new_month = _tm.tm_mon + value;
		auto new_year = _tm.tm_year + new_month / 12 + 1900;
		new_month = new_month % 12 + 1;
		auto daysInMonth = DaysInMonth(new_year, new_month);
		auto new_day = _tm.tm_mday < daysInMonth ? _tm.tm_mday : daysInMonth;
		return NFDateTime(_tm.tm_year, new_month, new_day, _tm.tm_hour, _tm.tm_min, _tm.tm_sec);
	}


	inline NFDateTime AddYears(const int value) const
	{
		auto new_year = _tm.tm_year + value + 1900;
		auto daysInMonth = DaysInMonth(new_year, _tm.tm_mon);
		auto new_day = _tm.tm_mday < daysInMonth ? _tm.tm_mday : daysInMonth;
		return NFDateTime(_tm.tm_year, _tm.tm_mon, new_day, _tm.tm_hour, _tm.tm_min, _tm.tm_sec);
	}

	inline bool Equals(const NFDateTime& value)
	{
		return *this == value;
	}

	inline bool IsDaylightSavingTime() const
	{
		return _tm.tm_isdst;
	}

	inline NFTimeSpan Subtract(const NFDateTime& value) const
	{
		return _totalSecond - value._totalSecond;
	}

	inline void Subtract(const NFTimeSpan& value)
	{
		_totalSecond -= value.totalSecond;
		UpdateTM();
	}

	//yyyy-MM-dd hh:mm:ss
	inline std::string ToString()
	{
		std::stringstream ss(std::stringstream::in | std::stringstream::out);
		ss << std::setfill('0') << std::setw(4) << Year() << "-" << std::setw(2) << Month() << "-" << std::setw(2) << Day();
		ss << " ";
		ss << std::setfill('0') << std::setw(2) << Hour() << ":" << std::setw(2) << Minute() << ":" << std::setw(2) << Second();

		return ss.str();
	}

	inline NFTimeSpan operator - (const NFDateTime& d) const
	{
		return Subtract(d);
	}

	inline bool operator ==(const NFDateTime &dt) const
	{
		return _totalSecond == dt._totalSecond;
	}

	inline bool operator !=(const NFDateTime &dt) const
	{
		return _totalSecond != dt._totalSecond;
	}

	inline bool operator <(const NFDateTime &dt) const
	{
		return _totalSecond < dt._totalSecond;
	}

	inline bool operator >(const NFDateTime &dt) const
	{
		return _totalSecond > dt._totalSecond;
	}

	inline bool operator <=(const NFDateTime &dt) const
	{
		return _totalSecond <= dt._totalSecond;
	}

	inline bool operator >=(const NFDateTime &dt) const
	{
		return _totalSecond >= dt._totalSecond;
	}
  
protected:
    int _totalSecond;
    std::tm _tm;

    void UpdateTM()
	{
		time_t t = static_cast<time_t>(_totalSecond);
		_tm = *std::gmtime(&t);
	}

    static std::string NameOfDay(::DayOfWeek day)
    {
        switch (day)
        {
            DAY_CASE(Monday);
            DAY_CASE(Tuesday);
            DAY_CASE(Wednesday);
            DAY_CASE(Thursday);
            DAY_CASE(Friday);
            DAY_CASE(Saturday);
            DAY_CASE(Sunday);

            default:
                //throw NFException("Day %d is not in valid weekday range ( %d - %d )", day, ::DayOfWeek::Sunday, ::DayOfWeek::Saturday);
                break;
        }
    }

    static std::string NameOfMonth(int month)
    {
        switch (static_cast<::Month>(month))
        {
            MONTH_CASE(January);
            MONTH_CASE(February);
            MONTH_CASE(March);
            MONTH_CASE(April);
            MONTH_CASE(May);
            MONTH_CASE(June);
            MONTH_CASE(July);
            MONTH_CASE(August);
            MONTH_CASE(September);
            MONTH_CASE(October);
            MONTH_CASE(November);
            MONTH_CASE(December);

            default:
                //throw NFException("Month %d is not in valid range ( %d - %d )", month, ::Month::January, ::Month::December);
                break;
        }
    }
    static std::string DaySuffix(int day)
    {
        switch (day)
        {
            case 1:
            case 21:
            case 31:
                return "st";

            case 2:
            case 22:
                return "nd";

            case 3:
            case 23:
                return "rd";

            default:
                return "th";
        }
    }
};

#endif
