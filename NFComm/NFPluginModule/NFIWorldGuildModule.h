// -------------------------------------------------------------------------
//    @FileName      :    NFIWorldGuildModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFIWorldGuildModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_WORLD_GUILD_MODULE_H
#define _NFI_WORLD_GUILD_MODULE_H

#include "NFILogicModule.h"

class NFIWorldGuildModule
    : public NFILogicModule
{
public:
	virtual const NFGUID& CreateGuild( const NFGUID& self, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP) = 0;
	virtual bool JoinGuild(const NFGUID& self, const NFGUID& xGuildID) = 0;
	virtual bool LeaveGuild(const NFGUID& self, const NFGUID& xGuildID) = 0;
	virtual bool UpGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;
	virtual bool DownGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;
	virtual bool KickGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;
	
	virtual bool GetGuildBaseInfo(const NFGUID& self, const NFGUID& xGuildID) = 0;
	virtual bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID) = 0;
	virtual bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember) = 0;

    virtual bool GetOnlineMember(const NFGUID& self, const NFGUID& xGuild, NFCDataList& varMemberList, NFCDataList& varGameList) = 0;
    virtual bool MemberOnline(const NFGUID& self, const NFGUID& xGuild, const int& nGameID) = 0;
    virtual bool MemberOffeline(const NFGUID& self, const NFGUID& xGuild) = 0;

private:
};

#endif