// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldGuildModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldGuildModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_WORLD_GUILD_MODULE_H_
#define _NFC_WORLD_GUILD_MODULE_H_

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"

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
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

	virtual NFIDENTID CreateGuild(const NFIDENTID& self, const std::string& strName);
	virtual bool JoinGuild(const NFIDENTID& self, const NFIDENTID& xGuildID);
	virtual bool LeaveGuild(const NFIDENTID& self, const NFIDENTID& xGuildID);
	virtual bool UpGuildMmember(const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember);
	virtual bool DownGuildMmember(const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember);
	virtual bool KickGuildMmember(const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember);


	//////////////////////其次考虑参数////////////////////////////////////////////////////
	virtual bool GetGuildBaseInfo(const NFIDENTID& self, const NFIDENTID& xGuildID);
	virtual bool GetGuildMemberInfo(const NFIDENTID& self, const NFIDENTID& xGuildID);
	virtual bool GetGuildMemberInfo(const NFIDENTID& self, const NFIDENTID& xGuildID, const NFIDENTID& xMmember);


	//////////////////////////////////////////////////////////////////////////
protected:

protected:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
private:
};

#endif