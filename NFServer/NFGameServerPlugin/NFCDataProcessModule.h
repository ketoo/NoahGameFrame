// -------------------------------------------------------------------------
//    @FileName      :    NFCDataProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             £º    2013-10-03
//    @Module           :    NFCDataProcessModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_DATAPROCESS_MODULE_H_
#define _NFC_DATAPROCESS_MODULE_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIDataNoSqlModule.h"
#include "NFComm/NFPluginModule/NFIDataProcessModule.h"

class NFCDataProcessModule
    : public NFIDataProcessModule
{
public:

    NFCDataProcessModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCDataProcessModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    virtual int LoadDataFormNoSql( const NFIDENTID& self );
    virtual int SaveDataToNoSql( const NFIDENTID& self, bool bOffline = false );
#ifdef NF_USE_ACTOR
	virtual Theron::Address GetActorID(const NFIDENTID& self);
#endif

protected:
    virtual void Handler(const NFIActorMessage& message, const Theron::Address from);
    virtual void HandlerEx(const NFIActorMessage& message, const Theron::Address from);
    virtual void HandlerTrans(const NFIActorMessage& message, const Theron::Address from);
    virtual void HandlerLog(const NFIActorMessage& message, const Theron::Address from);

protected:
    int OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var );

    virtual int LoadProperty( const NFIDENTID& self, std::shared_ptr<NFIPropertyManager> pProManager );
    virtual int LoadRecord( const NFIDENTID& self, std::shared_ptr<NFIRecordManager> pRecord );

    virtual int SaveProperty( const NFIDENTID& self, std::shared_ptr<NFIPropertyManager> pProManager );
    virtual int SaveRecord( const NFIDENTID& self, std::shared_ptr<NFIRecordManager> pRecordManager );

private:

    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIDataNoSqlModule* m_pNoSqlModule;
    NFIGameLogicModule* m_pGameLogicModule;
};


#endif
