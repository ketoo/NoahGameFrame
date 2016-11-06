#include "NFTime.h"

NFINT64 NFTime::GetTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string NFTime::GetStr()
{
	std::string str;
	NFCalendar calendar;
	std::stringstream ss;
	ss << calendar.Get(CalendarType::YEAR) << "-" << calendar.Get(CalendarType::MONTH) + 1 << "-" << calendar.Get(CalendarType::DAY) << " " << calendar.Get(CalendarType::HOUR) << ":" << calendar.Get(CalendarType::MINUTE) << ":" << calendar.Get(CalendarType::SECOND);
	return ss.str();
}

std::string NFTime::GetStr_YMD()
{
	std::string str;
	NFCalendar calendar;
	std::stringstream ss;
	ss << calendar.Get(CalendarType::YEAR) << "-" << calendar.Get(CalendarType::MONTH) + 1 << "-" << calendar.Get(CalendarType::DAY);
	return ss.str();
}
std::string NFTime::GetStr_HMS()
{
	std::string str;
	NFCalendar calendar;
	std::stringstream ss;
	ss << calendar.Get(CalendarType::HOUR) << ":" << calendar.Get(CalendarType::MINUTE) << ":" << calendar.Get(CalendarType::SECOND);
	return ss.str();
}
std::string NFTime::GetStr_YMDHMSM()
{
	std::string str;
	NFCalendar calendar;
	std::stringstream ss;
	ss << calendar.Get(CalendarType::YEAR) << "-" << calendar.Get(CalendarType::MONTH) + 1 << "-" << calendar.Get(CalendarType::DAY) << " " << calendar.Get(CalendarType::HOUR) << ":" << calendar.Get(CalendarType::MINUTE) << ":" << calendar.Get(CalendarType::SECOND) << " " << calendar.Get(CalendarType::MILLISECOND);
	return ss.str();
}