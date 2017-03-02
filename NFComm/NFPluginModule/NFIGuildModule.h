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
    virtual const NFGUID& CreateGuild(const NFGUID& self, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP) = 0;
    virtual bool JoinGuild(const NFGUID& self, const NFGUID& xGuildID) = 0;
    virtual bool LeaveGuild(const NFGUID& self, const NFGUID& xGuildID) = 0;
    virtual bool UpGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;
    virtual bool DownGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;
    virtual bool KickGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;

    virtual bool GetOnlineMember(const NFGUID& self, const NFGUID& xGuild, NFDataList& varMemberList, NFDataList& varGameList) = 0;
    virtual bool MemberOnline(const NFGUID& self, const NFGUID& xGuild, const int& nGameID) = 0;
    virtual bool MemberOffline(const NFGUID& self, const NFGUID& xGuild) = 0;

private:
};

#endif