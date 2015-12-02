// -------------------------------------------------------------------------
//    @FileName      :    NFCDataProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             ��    2013-10-03
//    @Module           :    NFCDataProcessModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_DATAPROCESS_MODULE_H_
#define _NFC_DATAPROCESS_MODULE_H_

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIDataProcessModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"
#include "NFComm/NFPluginModule/NFIUUIDModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFMsgBase.pb.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFCDataProcessModule
    : public NFIDataProcessModule
{
public:

    NFCDataProcessModule( NFIPluginManager* p )
    {
        pPluginManager = p;

        mstrRoleTable = "Player";
        mstrAccountTable = "AccountInfo";
    }
    virtual ~NFCDataProcessModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

	virtual const NFIDENTID& CreateRole(const std::string& strAccount, const std::string& strName, const int nRace, const int nJob, const int nSex);
	virtual const bool DeleteRole(const std::string& strAccount, const NFIDENTID xID);
	virtual const NFIDENTID& GetChar(const std::string& strAccount, const std::vector<std::string>& xFieldVec, std::vector<std::string>& xValueVec);

	//////////////////////////////////////////////////////////////////////////

	virtual bool RegisterAutoSave(const std::string& strClassName);
	virtual const bool LoadDataFormSql( const NFIDENTID& self , const std::string& strClassName);
	virtual const bool SaveDataToSql( const NFIDENTID& self);

private:
	const bool AttachData( const NFIDENTID& self );
	const bool ConvertPBToRecord(const NFMsg::PlayerRecordBase& xRecordData, NF_SHARE_PTR<NFIRecord> xRecord);

    int OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

    void OnOnline(const NFIDENTID& self);
    void OnOffline(const NFIDENTID& self);

private:
    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIClusterModule* m_pClusterSQLModule;
	NFIUUIDModule* m_pUUIDModule;
	NFILogicClassModule* m_pLogicClassModule;
	NFILogModule* m_pLogModule;

private:
	NFMapEx<NFIDENTID, NFMapEx<std::string, std::string> > mtObjectCache;

    std::string mstrRoleTable;
    std::string mstrAccountTable;
};


#endif
