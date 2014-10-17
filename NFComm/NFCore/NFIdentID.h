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
            NFINT32 nIdent;     // 唯一ID
            union
            {
                struct
                {
                    NFINT16 nServerID;  // 服务器ID
                    NFINT16 nAreaID;    // 大区ID
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
        //__asm {RDTSC} // 32位下可以通过 64位下无法内嵌汇编
    }

    // 可以保证服务器内玩家的物品ID唯一
    // 注意：需要在物品交易的地方加上角色ID，凑成128位来判断，需要逻辑多做一步
    // 交易完成后删除交换的物品，重新生成物品的GUID，即可保证唯一性
    // 主要是为了防止合大区导致的GUID重复问题
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
