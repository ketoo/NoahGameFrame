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

class NFCWorldGuildDataModule
    : public NFIWorldGuildDataModule
{
public:
    NFCWorldGuildDataModule(NFIPluginManager* p)
    {
        pPluginManager = p;

        mstrGuildTalble = "GuildInfo";
        mstrGuildNameTalble = "GuildName";
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();

    virtual bool ExitGuild(const NFIDENTID& self, const std::string& strName, bool& bExit);
    virtual void  CheckLoadGuild(const NFIDENTID& self, const NFIDENTID& xGuild);
    virtual NF_SHARE_PTR<NFIObject> GetGuild(const NFIDENTID& xGuild);

    virtual const NFIDENTID CreateGuild(const NFIDENTID& xPlayeID, const std::string& strName);
    virtual const bool DeleteGuild(const NFIDENTID& xGuild);

protected:
    int OnGuildClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

protected:
   const bool LoadDataFormNoSql( const NFIDENTID& self );
   const bool SaveDataToNoSql( const NFIDENTID& self);
   const bool AttachData( const NFIDENTID& self );

protected:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIUUIDModule* m_pUUIDModule;
    NFIClusterModule* m_pClusterSQLModule;
    NFIGameLogicModule* m_pGameLogicModule;
    
private:
    struct RoleData 
    {
        RoleData()
        {
            strProperty = "";
            strRecord = "";
        }

        std::string strProperty;
        std::string strRecord;
    };

    NFMapEx<NFIDENTID, RoleData> mxRoleDataMap;

    std::string mstrGuildTalble;
    std::string mstrGuildNameTalble;
};

#endif