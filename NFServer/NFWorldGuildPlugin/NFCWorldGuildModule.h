// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldGuildModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldGuildModule
//
// -------------------------------------------------------------------------

#ifndef NFC_WORLD_GUILD_MODULE_H
#define NFC_WORLD_GUILD_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildDataModule.h"

class NFCWorldGuildModule
    : public NFIWorldGuildModule
{
public:
    NFCWorldGuildModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

	virtual const NFGUID& CreateGuild( const NFGUID& self, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP);
	virtual bool JoinGuild(const NFGUID& self, const NFGUID& xGuildID);
	virtual bool LeaveGuild(const NFGUID& self, const NFGUID& xGuildID);
	virtual bool UpGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember);
	virtual bool DownGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember);
	virtual bool KickGuildMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember);

    virtual bool GetOnlineMember(const NFGUID& self, const NFGUID& xGuild, NFCDataList& varMemberList, NFCDataList& varGameList);
    virtual bool MemberOnline(const NFGUID& self, const NFGUID& xGuild, const int& nGameID);
    virtual bool MemberOffeline(const NFGUID& self, const NFGUID& xGuild);

	//////////////////////其次考虑参数////////////////////////////////////////////////////
	virtual bool GetGuildBaseInfo(const NFGUID& self, const NFGUID& xGuildID);
	virtual bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID);
	virtual bool GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember);

protected:
    bool CheckPower(const NFGUID& self, const NFGUID& xGuildID, int nPowerType);

protected:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIWorldGuildDataModule* m_pWorldGuildDataModule;
};

#endif