#ifndef NF_COMMON_TIMER_H
#define NF_COMMON_TIMER_H

/**
filename:common_timer.h
author  :eliteYang
mail    :elite_yang@163.com
desc    :��ʱ����
*/

#include <stdint.h>
#include <time.h>
#include <string>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <sys/timeb.h>
#include "NFSingleton.h"
#else
#include <sys/time.h>
#include <utime.h>
#include <stddef.h>
#include <string.h>
#endif

#ifdef _MSC_VER
class CurrentTimeProvider : public NFSingleton<CurrentTimeProvider>
{
public:
    CurrentTimeProvider(): highResolutionAvailable(false), countPerMilliSecond(0), beginCount(0)
    {
        static LARGE_INTEGER systemFrequency;
        if (0 != QueryPerformanceFrequency(&systemFrequency))
        {
            highResolutionAvailable = true;
            countPerMilliSecond = systemFrequency.QuadPart / 1000;
            _timeb tb;
            _ftime_s(&tb);
            unsigned short currentMilli = tb.millitm;
            LARGE_INTEGER now;
            QueryPerformanceCounter(&now);
            beginCount = now.QuadPart - (currentMilli * countPerMilliSecond);
        }
    };
    int64_t getCurrentTime()
    {
        time_t tt;
        ::time(&tt);

        int64_t millisecond = 0;
        if (highResolutionAvailable)
        {
            LARGE_INTEGER qfc;
            QueryPerformanceCounter(&qfc);
            millisecond = (int)((qfc.QuadPart - beginCount) / countPerMilliSecond) % 1000;
        }
        return tt * 1000 + millisecond;
    }
private:
    bool highResolutionAvailable;
    int64_t countPerMilliSecond;
    int64_t beginCount;
};

#endif

class NFTimeSpan
{
public:
    NFTimeSpan() throw();

    //TimeGetTime��ʱ��ת��. �������뾫��
    NFTimeSpan(uint32_t dwTime /* */);

    NFTimeSpan(time_t time) throw();
    NFTimeSpan(uint32_t lDays, int nHours, int nMins, int nSecs) throw();

    int64_t GetDays() const throw();
    int64_t GetTotalHours() const throw();
    uint32_t GetHours() const throw();
    int64_t GetTotalMinutes() const throw();
    uint32_t GetMinutes() const throw();
    int64_t GetTotalSeconds() const throw();
    uint32_t GetSeconds() const throw();

    time_t GetTimeSpan() const throw();

    NFTimeSpan operator+(NFTimeSpan span) const throw();
    NFTimeSpan operator-(NFTimeSpan span) const throw();
    NFTimeSpan& operator+=(NFTimeSpan span) throw();
    NFTimeSpan& operator-=(NFTimeSpan span) throw();
    bool operator==(NFTimeSpan span) const throw();
    bool operator!=(NFTimeSpan span) const throw();
    bool operator<(NFTimeSpan span) const throw();
    bool operator>(NFTimeSpan span) const throw();
    bool operator<=(NFTimeSpan span) const throw();
    bool operator>=(NFTimeSpan span) const throw();
private:
    time_t m_timeSpan;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class NFTimeEx
{
public:
    enum
    {
        UNDEF_TIME = -1
    };
public:
    static NFTimeEx GetCurrentTime() throw()
    {
        return NFTimeEx(time(NULL));
    }

    NFTimeEx() throw();
    NFTimeEx(time_t time) throw();
    NFTimeEx(int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec, int nDST = -1);
    NFTimeEx(uint16_t wDosDate, uint16_t wDosTime, int nDST = -1);

    NFTimeEx& operator=(time_t time) throw();

    NFTimeEx& operator+=(NFTimeSpan span) throw();
    NFTimeEx& operator-=(NFTimeSpan span) throw();

    NFTimeSpan operator-(NFTimeEx time) const throw();
    NFTimeEx operator-(NFTimeSpan span) const throw();
    NFTimeEx operator+(NFTimeSpan span) const throw();

    bool operator==(NFTimeEx time) const throw();
    bool operator!=(NFTimeEx time) const throw();
    bool operator<(NFTimeEx time) const throw();
    bool operator>(NFTimeEx time) const throw();
    bool operator<=(NFTimeEx time) const throw();
    bool operator>=(NFTimeEx time) const throw();

    int GetYear() const throw();
    int GetMonth() const throw();
    int GetDay() const throw();
    int GetHour() const throw();
    int GetMinute() const throw();
    int GetSecond() const throw();
    int GetDayOfWeek() const throw();

    bool SameMinute(int64_t nTime);
    bool SameHour(int64_t nTime);
    bool SameDay(int64_t nTime);
    bool SameWeek(int64_t nTime);
    bool SameMonth(int64_t nTime);
    bool SameYear(int64_t nTime);

    static bool IsPassCurrentTime(time_t n64Time)   // �Ƿ������
    {
        NFTimeEx ItemTime(n64Time);
        return (ItemTime <= GetCurrentTime());
    }

    //second
    static time_t GetNowTime()
    {
        return GetCurrentTime().GetTime();
    }

    //msec
    static int64_t GetNowTimeMille()
    {
#ifdef _MSC_VER
        return CurrentTimeProvider::GetSingletonPtr()->getCurrentTime();
#else
        struct timeval start;
        gettimeofday(&start, NULL);
        int64_t nSec = start.tv_sec;
        int64_t nUSec = start.tv_usec;
        return nSec * 1000 + nUSec / 1000;
#endif
    }

    void Print(std::string& strTime)
    {
        struct tm* ptimeTemp = GetGmtTm();
        if (ptimeTemp)
        {
            char buf[255] = {0};
            strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ptimeTemp);
            strTime.assign(buf, strlen(buf));
        }

        std::cout << "Time = " << strTime << std::endl;
    }

protected:
    time_t GetTime() const throw();

    struct tm* GetGmtTm(struct tm* ptm) const;
    struct tm* GetLocalTm(struct tm* ptm) const;
#if !_SECURE_ATL
    //  _ATL_INSECURE_DEPRECATE("Pass an output time structure to Time::GetGmtTm")
    struct tm* GetGmtTm() const throw();
    //  _ATL_INSECURE_DEPRECATE("Pass an output time structure to Time::GetLocalTm")
    struct tm* GetLocalTm() const throw();
#endif


private:
    time_t m_time;
};

#endif // __COMMON_TIMER_H_
