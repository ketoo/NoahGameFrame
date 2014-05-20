#ifndef __NF_COMMON_TIMER_H__
#define __NF_COMMON_TIMER_H__

/**
filename:common_timer.h
author  :eliteYang
mail    :elite_yang@163.com
desc    :��ʱ����
*/

#include <stdint.h>
#include <time.h>


#if defined(_WIN32) || defined(_WIN64)
#else
#include <sys/time.h>
#include <utime.h>
#include <stddef.h>
#endif

extern int64_t HQ_TimeGetTime();

// ��64bit����֤ʱ�䲻��Խ��
 class GameTime
 {
 public:
     typedef int64_t Time64;
 
     static void   MyGetCurrentTime();
     static Time64 GetCurrentTime64();
 
     static bool   IsOverDeadLine( Time64 deadTime ); // �Ƿ���Ķ���ʱ��
     static bool   IsOverDeadLine( int64_t dwCurrentTime, Time64 n64DeadTime );
 
     // ʹ������������ж�ʱ��
     static bool   IsPassCurrentTime( int64_t dwBaseTime, uint32_t dwKeepTime );
     static bool   IsPassCurrentTime( int64_t dwCurrentTime, int64_t dwBaseTime, uint32_t dwKeepTime );
 
     // ���ʣ��ʱ��
     // dwCurrentTime ��ǰʱ��
     // dwBaseTime �ϴα���ʱ��
     // dwKeepTime  �ϴ�ʣ��ʱ��
     static uint32_t GetLeftKeepTime( int64_t dwCurrentTime, int64_t dwLastTime, uint32_t dwKeepTime );
 
     // �����Ϸʱ����˶��
     static int64_t GetPassTime( int64_t dwCurrentTime, int64_t dwLastTime );
 
 private:
     static bool   IsLessOrEqualCurrentTime( Time64 n64DeadTime                );
     static bool   IsLessOrEqualCurrentTime( int64_t dwCurrentTime, Time64 n64DeadTime );
 
 };

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class NFTimeSpan
{
public:
    NFTimeSpan() throw();


    //TimeGetTime��ʱ��ת��. �������뾫��
    NFTimeSpan( uint32_t dwTime /* */);

    NFTimeSpan( time_t time ) throw();
    NFTimeSpan( uint32_t lDays, int nHours, int nMins, int nSecs ) throw();

    int64_t GetDays() const throw();
    int64_t GetTotalHours() const throw();
    uint32_t GetHours() const throw();
    int64_t GetTotalMinutes() const throw();
    uint32_t GetMinutes() const throw();
    int64_t GetTotalSeconds() const throw();
    uint32_t GetSeconds() const throw();

    time_t GetTimeSpan() const throw();

    NFTimeSpan operator+( NFTimeSpan span ) const throw();
    NFTimeSpan operator-( NFTimeSpan span ) const throw();
    NFTimeSpan& operator+=( NFTimeSpan span ) throw();
    NFTimeSpan& operator-=( NFTimeSpan span ) throw();
    bool operator==( NFTimeSpan span ) const throw();
    bool operator!=( NFTimeSpan span ) const throw();
    bool operator<( NFTimeSpan span ) const throw();
    bool operator>( NFTimeSpan span ) const throw();
    bool operator<=( NFTimeSpan span ) const throw();
    bool operator>=( NFTimeSpan span ) const throw();
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
    // 	static bool IsValidFILETIME(const FILETIME& ft) throw();

    NFTimeEx() throw();
    NFTimeEx( time_t time ) throw();
    NFTimeEx( int nYear, int nMonth, int nDay, int nHour, int nMin, int nSec,
        int nDST = -1 );
    NFTimeEx( uint16_t wDosDate, uint16_t wDosTime, int nDST = -1 );

    NFTimeEx& operator=( time_t time ) throw();

    NFTimeEx& operator+=( NFTimeSpan span ) throw();
    NFTimeEx& operator-=( NFTimeSpan span ) throw();

    NFTimeSpan operator-( NFTimeEx time ) const throw();
    NFTimeEx operator-( NFTimeSpan span ) const throw();
    NFTimeEx operator+( NFTimeSpan span ) const throw();

    bool operator==( NFTimeEx time ) const throw();
    bool operator!=( NFTimeEx time ) const throw();
    bool operator<( NFTimeEx time ) const throw();
    bool operator>( NFTimeEx time ) const throw();
    bool operator<=( NFTimeEx time ) const throw();
    bool operator>=( NFTimeEx time ) const throw();

    struct tm* GetGmtTm( struct tm* ptm ) const;
    struct tm* GetLocalTm( struct tm* ptm ) const;
#if !_SECURE_ATL
    //	_ATL_INSECURE_DEPRECATE("Pass an output time structure to Time::GetGmtTm")
    struct tm* GetGmtTm() const throw();
    //	_ATL_INSECURE_DEPRECATE("Pass an output time structure to Time::GetLocalTm")
    struct tm* GetLocalTm() const throw();
#endif

    // 	bool GetAsSystemTime( SYSTEMTIME& st ) const throw();
    //	bool GetAsDBTIMESTAMP( DBTIMESTAMP& dbts ) const throw();

    time_t GetTime() const throw();

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

    //#ifndef _ATL_MIN_CRT
    //	// formatting using "C" strftime
    //	std::string Format( LPCTSTR pszFormat ) const;
    //	std::string FormatGmt( LPCTSTR pszFormat ) const;
    //	std::string Format( UINT nFormatID ) const;
    //	std::string FormatGmt( UINT nFormatID ) const;
    //#endif
    // for compatibility with MFC 3.x
    //	std::string Format(LPCSTR pFormat) const;
    //	std::string FormatGmt(LPCSTR pFormat) const;
    //	std::string Format();

    static bool IsPassCurrentTime( time_t n64Time ) // �Ƿ������
    {
        NFTimeEx ItemTime( n64Time );
        return ( ItemTime <= GetCurrentTime() );
    }

    static bool IsPassCurrentTime( time_t n64StartTime, time_t nSpaceTime )
    {
        NFTimeEx xTime( n64StartTime );
        NFTimeSpan xSpan( nSpaceTime );
        xTime += xSpan;

        return ( GetCurrentTime() >= xTime );
    }

    static time_t GetNowTime()
    {
        return GetCurrentTime().GetTime();
    }

private:
    time_t m_time;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

// ��ʵʱ������
class RealType
{
public:
    typedef int64_t ValueType ;
    typedef NFTimeEx ClassType ;
};

// ��Ϸʱ������
class GameType
{
public:
    typedef unsigned int ValueType ;
    typedef NFTimeEx ClassType ;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template< class T >
class NFTimerEx
{
protected:

    typedef typename T::ValueType ValueType;
    typedef typename T::ClassType ClassType;

public:
    NFTimerEx() : m_nStartTime( 0 ), m_nSpaceTime( 0 )
    {

    }

    // ��ʱ���Ƿ��Ѿ�����
    bool IsStart() const { return m_nStartTime != 0; }

    // ����Ƿ�����, ��������һ��
    bool CheckStart( ValueType nCurrentTime )
    {
        if ( !IsStart() )
        { return false; }

        NextTimer( nCurrentTime );
        return true;
    }

    // ����Ƿ�����, ��ֹͣ��ʱ��
    bool CheckStart()
    {
        if ( !IsStart() )
        { return false; }

        StopTimer();
        return true;
    }

    bool CheckStop()
    {
        DoneTimer();

        return !IsStart();
    }

    // ��ʱ������
    void StartTimer( ValueType nStartTime, ValueType nSpaceTime )
    {
        if ( m_nStartTime == 0 || m_nSpaceTime == 0 )
        {
            SetStartTime( nStartTime );
            SetSpaceTime( nSpaceTime );
        }
    }

    // ��һ�μ�ʱ
    void NextTimer( ValueType nValue ) { SetStartTime( nValue ); }

    // ��ʱ��ֹͣ
    void StopTimer() { m_nStartTime = 0; }

    // ���ü�ʱ�����ʱ��
    ValueType GetStartTime() const { return m_nStartTime; }
    ValueType GetSpaceTime() const { return m_nSpaceTime; }
    void SetSpaceTime( ValueType nValue ) { m_nSpaceTime = nValue; }
    void AddSpaceTime( ValueType nValue ) { m_nSpaceTime += nValue; }

    // �Ƿ��ʱ���( ֻ��ʱһ�� )
    bool DoneTimer()
    {
        bool bDone = DoneTimer_();
        if ( bDone )
        { StopTimer(); }

        return bDone;
    }

    // �Ƿ��ʱ���
    bool DoneTimer( ValueType nCurrentTime )
    {
        bool bDone = DoneTimer_();
        if ( bDone )
        { NextTimer( nCurrentTime ); }

        return bDone;
    }

private:
    // ���ÿ�ʼʱ��
    void SetStartTime( ValueType nValue )
    {
        if ( nValue == 0 )
        { nValue = 1; } // ��֤��ʼʱ�䲻Ϊ0

        m_nStartTime = nValue;
    }

    // �Ƿ��ʱ���
    bool DoneTimer_()
    {
        if ( m_nStartTime == 0 )
        { return false; }

        return ClassType::IsPassCurrentTime( m_nStartTime, m_nSpaceTime );
    }

private:
    ValueType m_nStartTime;     // ��ʼʱ��( 0 ��ʾ��δ��ʼ )
    ValueType m_nSpaceTime;     // ���ʱ��
};

typedef NFTimerEx< GameType > GameTimerEx;    // ��Ϸʱ�䶨ʱ��
typedef NFTimerEx< RealType > RealTimerEx;    // ��ʵʱ�䶨ʱ��

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//class GameTimer
//{
//public:
//    // ��ʼ��ʱ
//    void Start( uint32_t dwDelay )
//    {
//        m_dwStartTime = HQ_TimeGetTime();
//        m_dwDelayTime = dwDelay;
//        m_bStarted    = true;
//        m_bAlreadyUse = true;
//    }
//    // ����
//    void Run()
//    {
//        if(m_bStarted&&m_bAlreadyUse)
//        {
//            if( GameTime::IsPassCurrentTime( m_dwStartTime, m_dwDelayTime ) )
//            {
//                m_bStarted = false;
//            }
//        }
//    }
//
//    // �Ƿ񵹼�ʱ���
//    bool IsTimerDone()
//    {
//        if( m_bAlreadyUse )
//        {
//            if( !m_bStarted )
//            {
//                m_bAlreadyUse = false;
//                return true;
//            }
//
//            return false;
//        }
//
//        return true;
//    }
//
//    uint32_t TimeLeft()
//    {
//        return m_bStarted ? m_dwDelayTime-(HQ_TimeGetTime()-m_dwStartTime) : 0;
//    }
//
//    // ���˵�ʱ��
//    void BackupTime( int nBackupPercent )
//    {
//        if( m_bStarted )
//        {
//            m_dwStartTime += m_dwDelayTime * nBackupPercent / 100;
//
//            if( m_dwStartTime > HQ_TimeGetTime() )
//            {
//                m_dwStartTime = HQ_TimeGetTime();
//            }
//        }
//    }
//
//    //���ü�ʱ����
//    void Reset()
//    {
//        m_bAlreadyUse = false;
//    }
//
//    GameTimer()
//    {
//        m_bStarted = false;
//        m_bAlreadyUse = false;
//        m_dwDelayTime = 0;
//        m_dwStartTime = 0;
//    }
//private:
//    uint32_t m_dwStartTime;
//    uint32_t m_dwDelayTime;
//    bool m_bStarted;
//    bool m_bAlreadyUse;
//};


#endif // __COMMON_TIMER_H__
