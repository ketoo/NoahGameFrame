// -------------------------------------------------------------------------
//    @FileName         :    NFCWorldGuildDataModule.h
//    @Author           :    chuanbo.guo
//    @Date             :    2015-06-06 16:51
//    @Module           :    NFCWorldGuildDataModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_WORLD_GUILD_DATA_MODULE_H_
#define _NFC_WORLD_GUILD_DATA_MODULE_H_

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIWorldGuildDataModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFMessageDefine/NFMsgBase.pb.h"

class NFCWorldGuildDataModule
    : public NFIWorldGuildDataModule
{
public:
    NFCWorldGuildDataModule(NFIPluginManager* p)
    {
        pPluginManager = p;

        mstrGuildTalble = "Guild";
        mstrGuildNameTalble = "GuildName";
        mContainerID = 1;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

    virtual bool ExitGuild(const NFIDENTID& self, const std::string& strName, bool& bExit);
    virtual void  CheckLoadGuild(const NFIDENTID& self, const NFIDENTID& xGuild);
    virtual NF_SHARE_PTR<NFIObject> GetGuild(const NFIDENTID& xGuild);

    virtual bool GetPlayerInfo(const NFIDENTID& self, std::string& strRoleName, int& nLevel, int& nJob , int& nDonation , int& nVIP);

    virtual const NFIDENTID CreateGuild(const NFIDENTID& xPlayeID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP, const int nOffLine = 1, const int nPower = NFMsg::GUILD_POWER_TYPE_PRESIDENT);
    virtual const bool DeleteGuild(const NFIDENTID& xGuild);
    virtual bool GetGuild(const NFIDENTID& self, NFIDENTID& xGuild);
    virtual bool SetGuild( const NFIDENTID& self, const NFIDENTID& xGuild );
    virtual bool SearchGuild(const NFIDENTID& self, const std::string& strName, std::vector<SearchGuildObject>& xList);
    virtual bool GetGuildInfo( const NFIDENTID& self, const NFIDENTID& xGuild, SearchGuildObject& xGuildInfo );
    virtual bool CanLoadMysqlData( const NFIDENTID& self );

protected:
    int OnGuildClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

protected:
   const bool LoadDataFormNoSql( const NFIDENTID& self );
   const bool SaveDataToNoSql( const NFIDENTID& self);
   const bool ConvertPBToRecord(const NFMsg::PlayerRecordBase& xRecordData, NF_SHARE_PTR<NFIRecord> xRecord);   

protected:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIClusterModule* m_pClusterSQLModule;    
    
private:
    std::string mstrGuildTalble;
    std::string mstrGuildNameTalble;
    int mContainerID;
};

#endif