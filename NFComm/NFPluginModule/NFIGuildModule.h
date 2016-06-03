// -------------------------------------------------------------------------
//    @FileName      :    NFIGuildModule.h
//    @Author           :    ChuanBo.Guo
//    @Date             :    2016-01-30 08:51
//    @Module           :   NFIGuildModule
//
// -------------------------------------------------------------------------

#ifndef NFI_GUILD_MODULE_H
#define NFI_GUILD_MODULE_H

#include "NFILogicModule.h"

class NFIGuildModule
    : public NFILogicModule
{
public:
    virtual const NFGUID& CreateGuild(const NFGUID& self, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP) = 0;
    virtual bool JoinGuild(const NFGUID& self, const NFGUID& xGuildID) = 0;
    virtual bool LeaveGuild(const NFGUID& self, const NFGUID& xGuildID) = 0;
    virtual bool UpGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;
    virtual bool DownGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;
    virtual bool KickGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;

    virtual bool GetGuildBaseInfo(const NFGUID& xGuildID, NF_SHARE_PTR<NFIPropertyManager>& pPropertyManager) = 0;
    virtual bool GetGuildMemberInfo(const NFGUID& xGuildID, NF_SHARE_PTR<NFIRecord>& pMemberRecord) = 0;
    virtual bool GetGuildMemberInfo(const NFGUID& xGuildID, const NFGUID& xMmember, NFIDataList& varMemeberInfo) = 0;
    virtual bool GetGuildMemberIDList(const NFGUID& xGuildID, std::map<std::string, int>& xMemberPos) = 0;

    virtual bool GetOnlineMember(const NFGUID& self, const NFGUID& xGuild, NFCDataList& varMemberList, NFCDataList& varGameList) = 0;
    virtual bool MemberOnline(const NFGUID& self, const NFGUID& xGuild, const int& nGameID) = 0;
    virtual bool MemberOffeline(const NFGUID& self, const NFGUID& xGuild) = 0;

private:
};

#endif