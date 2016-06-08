// -------------------------------------------------------------------------
//    @FileName         :    NFCGuildDataModule.h
//    @Author           :    chuanbo.guo
//    @Date             :    2015-06-06 16:51
//    @Module           :    NFCGuildDataModule
//
// -------------------------------------------------------------------------

#ifndef NFC_GUILD_DATA_MODULE_H
#define NFC_GUILD_DATA_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGuildDataModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFMessageDefine/NFMsgBase.pb.h"
#include "NFComm/NFPluginModule/NFIPlayerMysqlModule.h"
#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIMysqlModule.h"

class NFCGuildDataModule
    : public NFIGuildDataModule
{
public:
    NFCGuildDataModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

    virtual bool ExitGuild(const NFGUID& self, const std::string& strName, bool& bExit);
    virtual void  CheckLoadGuild(const NFGUID& self, const NFGUID& xGuild);
    virtual NF_SHARE_PTR<NFIObject> GetGuild(const NFGUID& xGuild);

    virtual bool GetPlayerInfo(const NFGUID& self, std::string& strRoleName, int& nLevel, int& nJob , int& nDonation , int& nVIP);
    virtual bool GetPlayerGuild(const NFGUID& self, NFGUID& xGuild);
    virtual bool GetPlayerGameID( const NFGUID& self, int& nGameID );

    virtual const NFGUID CreateGuild(const NFGUID& xPlayeID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP, const int nOffLine = 1, const int nPower = NFMsg::GUILD_POWER_TYPE_PRESIDENT);
    virtual const bool DeleteGuild(const NFGUID& xGuild);

    virtual bool SearchGuild(const NFGUID& self, const std::string& strName, std::vector<SearchGuildObject>& xList);
    virtual bool GetGuildInfo( const NFGUID& self, const NFGUID& xGuild, SearchGuildObject& xGuildInfo );

protected:
    int OnGuildClassEvent( const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );
    int OnSaveGuildheartEvent(const NFGUID& self , const std::string& strHeartName, const float fTime, const int nCount);
    void HandleLoadGuildSuccess(const NFGUID& xGuild, const int nResult, const std::string& strUseData);

protected:
    NFIKernelModule* m_pKernelModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIMysqlModule* m_pMysqlModule;    
	NFIPlayerMysqlModule* m_pPlayerMysqlModule;
    NFICommonConfigModule* m_pCommonConfigModule;
    
private:
};

#endif