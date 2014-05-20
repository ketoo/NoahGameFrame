// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseNet_ClientModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCDataBaseNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_DATABASENET_CLIENT_MODULE_H_
#define _NFC_DATABASENET_CLIENT_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFSvrMsgType.h"
#include "NFComm/NFMessageDefine/NFClientMsgType.h"
#include "NFComm/NFPluginModule/NFIShareMemoryModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIDataBaseModule.h"
#include "NFComm/NFCore/GTPropertyManager.h"
#include "NFComm/NFCore/GTRecordManager.h"

class NFCDataBaseNet_ClientModule
    : public NFILogicModule,
  public NFCNet
{
public:
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    virtual bool AfterInit();
    virtual bool BeforeShut();

    virtual  bool DispatcherSystemEvents(RakNet::MessageID ucEventID, RakNet::SystemAddress sAddress);
    virtual  bool DispatcherPacket(NFIPacket* pMsgPacket);
    virtual  bool DispatcherPacket(RakNet::Packet* pPacket, const std::string& strMsg)
    {
        return true;
    }
protected:

    void  CreateShareMemory();

    void LoadRoleDataProcess(NFIPacket* pMsgPacket);
    void SaveRoleDataProcess(NFIPacket* pMsgPacket);

    void Register();
    void UnRegister();

protected:
    int LoadRoleData(const std::string& strRoleName);

    int LoadRoleDataProperty(const std::string& strRoleName);
    int LoadRoleDataRecord(const std::string& strRoleName);

    bool SaveRoleData(const std::string& strRoleName);


private:

    static int mnDataBaseID;
    static char* mstrsShareName;

    static NFIShareMemoryModule* m_pShareMemoryModule;
    static NFILogicClassModule* m_pLogicClassModule;
    static NFIDataBaseModule* m_pDataBaseModule;
    static NFIKernelModule* m_pKernelModule;

    static NFCDataBaseNet_ClientModule* m_pThis;
};

#endif