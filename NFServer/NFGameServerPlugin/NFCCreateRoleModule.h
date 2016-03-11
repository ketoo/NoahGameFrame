// -------------------------------------------------------------------------
//    @FileName      :    NFCCreateRoleModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-03-29
//    @Module           :    NFCCreateRoleModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_CREATEROLE_MODULE_H_
#define _NFC_CREATEROLE_MODULE_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIDataNoSqlModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
//#include "NFComm/NFPluginModule/NFIShareMemoryModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"


class NFCCreateRoleModule
    : public NFILogicModule
{
public:
    NFCCreateRoleModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFCCreateRoleModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();

protected:
    static int OnLoadRoleBeginEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    static int OnLoadRoleFinalEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    static int OnCreateRoleEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    static int OnDeleteRoleEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

    static int OnAcountDisConnectEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var);

private:

    //�½��������Ӷ��󣬵ȴ������Լ�����֤KEY��KEY��֤��ɾ��
    //-1
    static int mnConnectContainer;

    //ѡ�˴�������
    //-3
    static int mnRoleHallContainer;

    static NFIGameLogicModule* m_pGameLogicModule;
    static NFIEventProcessModule* m_pEventProcessModule;
    //static NFIDataBaseModule* m_pDataBaseModule;
    static NFIDataNoSqlModule* m_pNoSqlModule;
    static NFIKernelModule* m_pKernelModule;
    static NFIElementInfoModule* m_pElementInfoModule;
    static NFCCreateRoleModule* m_pThis;
};

#endif
