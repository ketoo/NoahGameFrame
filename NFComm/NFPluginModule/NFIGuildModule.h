// -------------------------------------------------------------------------
//    @FileName      :    NFIGuildModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2016-01-30 08:51
//    @Module           :   NFIGuildModule
//
// -------------------------------------------------------------------------

#ifndef NFI_GUILD_MODULE_H
#define NFI_GUILD_MODULE_H

#include "NFIModule.h"

class NFIGuildModule
    : public NFIModule
{
public:
    enum MEMBER_TYPE
    {
        MT_PRESIDENT,
        MT_OLDER,
        MT_ELITE,
        MT_MEMBER,
    };

};

#endif