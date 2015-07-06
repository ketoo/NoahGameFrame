// -------------------------------------------------------------------------
//    @FileName      :    NFCObjectSaveModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCObjectSaveModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_OBJECTSAVE_MODULE_H_
#define _NFC_OBJECTSAVE_MODULE_H_

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIObjectSaveModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIClusterModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"

class NFCObjectSaveModule : public NFIObjectSaveModule
{
public:
    NFCObjectSaveModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();
    virtual bool BeforeShut();

    virtual bool RegisterAutoSave(const std::string& strClassName);
    virtual const bool LoadDataFormNoSql( const NFIDENTID& self , const std::string& strClassName);
    virtual const bool SaveDataToNoSql( const NFIDENTID& self);

private:
    const bool AttachData( const NFIDENTID& self );
    int OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );
    const bool ConvertPBToRecord(const NFMsg::PlayerRecordBase& xRecordData, NF_SHARE_PTR<NFIRecord> xRecord);

private:
    NFIKernelModule* m_pKernelModule;
    NFIEventProcessModule* m_pEventProcessModule;
    NFIClusterModule* m_pClusterSQLModule;
    NFILogicClassModule* m_pLogicClassModule;

    NFMapEx<NFIDENTID, NFMapEx<std::string, std::string> > mtObjectCache;
};

#endif