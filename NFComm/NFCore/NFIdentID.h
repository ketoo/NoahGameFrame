// -------------------------------------------------------------------------
//    @FileName         :    NFIDENTID.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-11
//    @Module           :    NFIDENTID
//
// -------------------------------------------------------------------------

#ifndef _NF_IDENTID_H_
#define _NF_IDENTID_H_

#include "NFPlatform.h"
#include "NFCMemory .h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>

//#pragma pack(push, 1)

struct NFIDENTID
{
    union
    {
        struct
        {
            NFINT64 nData64;
        };

        struct
        {
            NFINT32 nIdent;     // ΨһID
            union
            {
                struct
                {
                    NFINT16 nServerID;  // ������ID
                    NFINT16 nAreaID;    // ����ID
                };


                struct
                {
                    NFINT32 nSerial;
                };
            };
        };
    };

    NFIDENTID()
    {
        nData64 = 0;
    }

    NFIDENTID(NFINT32 nData)
    {
        nAreaID = 0;
        nServerID = 0;
        nIdent = nData;
    }
    NFIDENTID(unsigned int nData)
    {
        nData64 = nData;
    }
    NFIDENTID(NFINT64 nData)
    {
        nData64 = nData;
    }

    NFIDENTID(NFINT32 serial, NFINT32 ident)
    {
        nSerial = serial;
        nIdent = ident;
    }

    NFIDENTID(NFINT16 area, NFINT16 server, NFINT32 ident)
    {
        nAreaID = area;
        nServerID = server;
        nIdent = ident;
    }

    //     NFIDENTID( const char* strIdent )
    //     {
    //         nData64 = boost::lexical_cast<NFINT64>( strIdent );
    //     }

    bool IsNull() const
    {
        return 0 == nData64;
    }

    bool operator == (const NFIDENTID& id) const
    {
        return this->nData64 == id.nData64;
    }

    bool operator != (const NFIDENTID& id) const
    {
        return this->nData64 != id.nData64;
    }

    bool operator < (const NFIDENTID& id) const
    {
        return this->nData64 < id.nData64;
    }

    static std::string ToString(NFIDENTID self)
    {
        return boost::lexical_cast<std::string>(self.nData64);
    }

    static NFINT64 GetCPUTime()
    {

#if NF_PLATFORM == NF_PLATFORM_WIN
        return __rdtsc(); // Generates the rdtsc instruction, which returns the processor time stamp. The processor time stamp records the number of clock cycles since the last reset
#else
        return 0;

#endif
        //__asm {RDTSC} // 32λ�¿���ͨ�� 64λ���޷���Ƕ���
    }

    // ���Ա�֤����������ҵ���ƷIDΨһ
    // ע�⣺��Ҫ����Ʒ���׵ĵط����Ͻ�ɫID���ճ�128λ���жϣ���Ҫ�߼�����һ��
    // ������ɺ�ɾ����������Ʒ������������Ʒ��GUID�����ɱ�֤Ψһ��
    // ��Ҫ��Ϊ�˷�ֹ�ϴ������µ�GUID�ظ�����
    static NFIDENTID CreateGUID()
    {
        time_t ltime;
        time(&ltime);

        NFINT64 value = ltime;
        value <<= 31;
        value += NFINT32(GetCPUTime());
        return value;
    }
};
//#pragma pack(pop)
#endif
