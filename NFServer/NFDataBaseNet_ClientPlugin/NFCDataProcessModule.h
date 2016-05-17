// -------------------------------------------------------------------------
//    @FileName      :    NFCDataProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             £º    2013-10-03
//    @Module           :    NFCDataProcessModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_DATAPROCESS_MODULE_H
#define NFC_DATAPROCESS_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIDataProcessModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

class NFCDataProcessModule
    : public NFIDataProcessModule
{
public:

    NFCDataProcessModule(NFIPluginManager* p)
    {
        pPluginManager = p;

        mstrRoleTable = "Player";
        mstrAccountTable = "AccountInfo";
    }
    virtual ~NFCDataProcessModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual const NFGUID CreateRole(const std::string& strAccount, const std::string& strName, const int nRace, const int nJob, const int nSex);
    virtual const bool DeleteRole(const std::string& strAccount, const NFGUID xID);
    virtual const NFGUID GetChar(const std::string& strAccount, const std::vector<std::string>& xFieldVec, std::vector<std::string>& xValueVec);

    //////////////////////////////////////////////////////////////////////////

    virtual bool RegisterAutoSave(const std::string& strClassName);
    virtual const bool LoadDataFormSql(const NFGUID& self , const std::string& strClassName);
    virtual const bool SaveDataToSql(const NFGUID& self);

private:
    const bool AttachData(const NFGUID& self);
    const bool ConvertPBToRecord(const NFMsg::PlayerRecordBase& xRecordData, NF_SHARE_PTR<NFIRecord> xRecord);

    int OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var);

    void OnOnline(const NFGUID& self);
    void OnOffline(const NFGUID& self);

private:
    NFIKernelModule* m_pKernelModule;
    NFIClusterModule* m_pClusterSQLModule;
    NFIUUIDModule* m_pUUIDModule;
    NFILogicClassModule* m_pLogicClassModule;
    NFILogModule* m_pLogModule;
    NFIElementInfoModule* m_pElementInfoModule;

private:
    NFMapEx<NFGUID, NFMapEx<std::string, std::string> > mtObjectCache;

    std::string mstrRoleTable;
    std::string mstrAccountTable;
};


#endif
