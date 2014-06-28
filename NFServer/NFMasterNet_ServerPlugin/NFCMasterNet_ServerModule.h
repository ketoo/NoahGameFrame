// -------------------------------------------------------------------------
//    @FileName      :    NFCMasterNet_ServerModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCMasterNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef _NFC_MASTERNET_SERVER_MODULE_H_
#define _NFC_MASTERNET_SERVER_MODULE_H_

//  the cause of sock'libariy, thenfore "NFCNet.h" much be included first.

#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIMasterNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"

class NFCMasterNet_ServerModule
	: public NFINetModule
{
public:
	NFCMasterNet_ServerModule(NFIPluginManager* p)
	{
		pPluginManager = p;
	}

	virtual bool Init();
	virtual bool Shut();

	virtual bool AfterInit();
	virtual bool Execute(const float fLasFrametime, const float fStartedTime);

	virtual void LogRecive(const char* str){}
	virtual void LogSend(const char* str){}

protected:

	int OnRecivePack(const NFIPacket& msg);
	int OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent);

	//���Ӷ�ʧ,ɾ2��(���Ӷ����ʺŶ���)
	void OnClientDisconnect(const int nAddress);
	//������
	void OnClientConnected(const int nAddress);
	
protected:
	//���������ע�ᣬˢ����Ϣ
	int OnWorldRegisteredProcess(const NFIPacket& msg);
	int OnWorldUnRegisteredProcess(const NFIPacket& msg);
	int OnRefreshWorldInfoProcess(const NFIPacket& msg);

	//////////////////////////////////////////////////////////////////////////
	//��¼������ע�ᣬˢ����Ϣ
	int OnLoginRegisteredProcess(const NFIPacket& msg);
	int OnLoginUnRegisteredProcess(const NFIPacket& msg);
	int OnRefreshLoginInfoProcess(const NFIPacket& msg);

	//ѡ�������������Ϣ
	int OnSelectWorldProcess(const NFIPacket& msg);
	int OnSelectServerResultProcess(const NFIPacket& msg);

	//////////////////////////////////////////////////////////////////////////

	void SynWorldToLogin();

private:

    struct ServerData 
    {
        ServerData()
        {
            pData = new NFMsg::ServerInfoReport();
            nFD = 0;
        }
        ~ServerData()
        {
            nFD = 0;
            delete pData;
            pData = NULL;
        }

        int nFD;
        NFMsg::ServerInfoReport* pData;
    };

private:
    //serverid,data
    NFMap<int, ServerData> mWorldMap;
    NFMap<int, ServerData> mLoginMap;

	
	NFIElementInfoModule* m_pElementInfoModule;
	NFILogicClassModule* m_pLogicClassModule;
	NFIEventProcessModule* m_pEventProcessModule;
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;
};

#endif