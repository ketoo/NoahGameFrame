#include "NFTime.h"

NFTime::NFTime()
{

}
NFTime::NFTime(NFINT64 nTime)
{
	mnTime = nTime;
}
//NFTime::NFTime(std::string strTime)
//{
//	std::vector<std::string> cells;
//	split(strTime, cells, " ");
//	if (cells.size() != 2)
//	{
//		return;
//	}
//	std::vector<std::string> cellsYMD;
//	split(cells.at(0), cellsYMD, "-");
//	if (cellsYMD.size() != 3)
//	{
//		return;
//	}
//	std::vector<std::string> cellsHMS;
//	split(cells.at(1), cellsHMS, ":");
//	if (cellsHMS.size() != 3)
//	{
//		return;
//	}
//	int year = lexical_cast<int>(cellsYMD[0]);
//	int month = lexical_cast<int>(cellsYMD[1]);
//	int day = lexical_cast<int>(cellsYMD[2]);
//	int hour = lexical_cast<int>(cellsHMS[0]);
//	int minute = lexical_cast<int>(cellsHMS[1]);
//	int second = lexical_cast<int>(cellsHMS[2]);
//	//mnTime = year*NFTime::WEEK
//}

NFINT64 NFTime::GetTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string NFTime::GetStr()
{
	std::string str;
	NFCalendar calendar;
	std::stringstream ss;
	ss << calendar.Get(NFCalendar::TYPE::YEAR) << "-" << calendar.Get(NFCalendar::TYPE::MONTH) + 1 << "-" << calendar.Get(NFCalendar::TYPE::DAY) << " " << calendar.Get(NFCalendar::TYPE::HOUR) << ":" << calendar.Get(NFCalendar::TYPE::MINUTE) << ":" << calendar.Get(NFCalendar::TYPE::SECOND);
	return ss.str();
}

std::string NFTime::GetStr_YMD()
{
	std::string str;
	NFCalendar calendar;
	std::stringstream ss;
	ss << calendar.Get(NFCalendar::TYPE::YEAR) << "-" << calendar.Get(NFCalendar::TYPE::MONTH) + 1 << "-" << calendar.Get(NFCalendar::TYPE::DAY);
	return ss.str();
}
std::string NFTime::GetStr_HMS()
{
	std::string str;
	NFCalendar calendar;
	std::stringstream ss;
	ss  << calendar.Get(NFCalendar::TYPE::HOUR) << ":" << calendar.Get(NFCalendar::TYPE::MINUTE) << ":" << calendar.Get(NFCalendar::TYPE::SECOND);
	return ss.str();
}
std::string NFTime::GetStr_YMDHMSM()
{
	std::string str;
	NFCalendar calendar;
	std::stringstream ss;
	ss << calendar.Get(NFCalendar::TYPE::YEAR) << "-" << calendar.Get(NFCalendar::TYPE::MONTH) + 1 << "-" << calendar.Get(NFCalendar::TYPE::DAY) << " " << calendar.Get(NFCalendar::TYPE::HOUR) << ":" << calendar.Get(NFCalendar::TYPE::MINUTE) << ":" << calendar.Get(NFCalendar::TYPE::SECOND)<<" "<<calendar.Get(NFCalendar::TYPE::MILLISECOND);
	return ss.str();
}