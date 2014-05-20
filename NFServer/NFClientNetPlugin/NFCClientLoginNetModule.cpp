// -------------------------------------------------------------------------
//    @FileName      :    NFCClientNetModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCClientLoginNetModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCClientLoginNetModule.h"
#include "NFClientNetPlugin.h"

extern NFIPluginManager* pPluginManager;

NFIKernelModule* NFCClientLoginNetModule::m_pKernelModule = NULL;
NFIEventProcessModule* NFCClientLoginNetModule::m_pEventProcessModule = NULL;
NFCClientLoginNetModule* NFCClientLoginNetModule::m_pThis = NULL;

int NFCClientLoginNetModule::mnClientConnectContainer = -2;

NFCClientLoginNetModule::NFCClientLoginNetModule()
{

}

NFCClientLoginNetModule::~NFCClientLoginNetModule()
{

}

bool NFCClientLoginNetModule::Init()
{
    m_pThis = this;
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pEventProcessModule);
    // 连接master server
    Initialization(false, "NFClientNetPlugin.cfg");

    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_LOGIN, OnLoginEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_LOGOUT, OnLogOutEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_DISCONNECT, OnDisconnectEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_SELECT_SERVER, OnSelectServerEvent);

    return true;
}

bool NFCClientLoginNetModule::Shut()
{
    Final();

    return true;
}

bool NFCClientLoginNetModule::DispatcherSystemEvents(RakNet::MessageID ucEventID, RakNet::SystemAddress sAddress)
{
    switch (ucEventID)
    {
        case ID_CONNECTION_REQUEST_ACCEPTED:
        {
            printf("NFClient_Net connect successed %s\n", sAddress.ToString());
        }
        break;
        case ID_CONNECTION_LOST:
        {
            printf("Client server close %s\n", sAddress.ToString());
        }
        break;
        case ID_DISCONNECTION_NOTIFICATION:
        {
            printf("NFClient_Net disconnected %s\n", sAddress.ToString());
        }
        break;
    }
    return true;
}

bool NFCClientLoginNetModule::DispatcherPacket(NFIPacket* pMsgPacket)
{
    unsigned int unMsgID = pMsgPacket->GetMessageId();
    switch (unMsgID)
    {
            //  case KERNEL_MSG_CONTAIN_SPRING:
            //  case KERNEL_MSG_CONTAIN_ENDSPRING:
            //      OnContainProcess( pMsgPacket );
            //      break;

        case KERNEL_MSG_OBJECT_SPRING:
        case KERNEL_MSG_OBJECT_ENDSPRING:
            OnObjectProcess(pMsgPacket);
            break;

        case KERNEL_MSG_PROPERTY_INT:
            OnObjectPropertyIntProcess(pMsgPacket);
            break;

        case KERNEL_MSG_PROPERTY_FLOAT:
            OnObjectPropertyFloatProcess(pMsgPacket);
            break;

        case KERNEL_MSG_PROPERTY_DOUBLE:
            OnObjectPropertyDoubleProcess(pMsgPacket);
            break;

        case KERNEL_MSG_PROPERTY_STRING:
            OnObjectPropertyStringProcess(pMsgPacket);
            break;

        case KERNEL_MSG_PROPERTY_OBJECT:
            OnObjectPropertyObjectProcess(pMsgPacket);
            break;

        case LTW_WANTTO_CONNECT_WORLD_RESULT:
            OnSelectWorldResultProcess(pMsgPacket);
            break;

        default:
            printf("NFNet || 非法消息:unMsgID=%d\n", pMsgPacket->GetMessageId());
            break;
    }

    return true;
}

bool NFCClientLoginNetModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    Update(fLasFrametime, fStartedTime);

    return true;
}

bool NFCClientLoginNetModule::Disconnected(const std::string& strAccount)
{
    return true;
}


int NFCClientLoginNetModule::OnSelectWorldResultProcess(NFIPacket* pMsgPacket)
{
    _tagLTW_WANTTO_CONNECT_WORLD_RESULT connectResult;

    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    connectResult.DeCode(*pBitStream);

    NFCValueList var;
    var << connectResult._nWorldID << connectResult._szSenderAddress << connectResult._nLoginID << connectResult._szAccount << connectResult._szWorldAddress << connectResult._nPort << connectResult._szWorldKey;
    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_SELECT_SERVER_RESULTS, var);

    return 0;
}

int NFCClientLoginNetModule::OnLoginEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (3 != var.GetCount())
    {
        return -1;
    }

    const char* pstrAddress = var.StringVal(0);
    const char* pstrAccount = var.StringVal(1);
    const char* pstrPassword = var.StringVal(2);

    _tagCTL_LOGIN clientToLogin;
    strcpy_s(clientToLogin._szAccount, pstrAccount);
    strcpy_s(clientToLogin._szPassword, pstrPassword);

    RakNet::BitStream oStream;
    clientToLogin.EnCode(oStream);
    m_pThis->SendBitStream(&oStream);

    return 0;
}

int NFCClientLoginNetModule::OnLogOutEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    return 0;
}

int NFCClientLoginNetModule::OnDisconnectEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    return 0;
}

int NFCClientLoginNetModule::OnObjectProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_OBJECT_SPRING objectSpring;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    objectSpring.DeCode(*pBitStream);

    if (objectSpring._nGroupID < 0)
    {
        //小余0为容器，0为所有均可见，1+为分组
        m_pKernelModule->CreateContainer(objectSpring._nContainer, "");
    }
    else
    {
        NFIDENTID container;
        if (!m_pKernelModule->ExistContainer(objectSpring._nContainer))
        {
            m_pKernelModule->CreateContainer(objectSpring._nContainer, "");
        }

        m_pKernelModule->CreateObject(objectSpring._nObjectID, objectSpring._nContainer, objectSpring._nGroupID, objectSpring._szClassName, objectSpring._szConfigName, NFCValueList());
    }

    return 0;
}

int NFCClientLoginNetModule::OnObjectPropertyIntProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_PROPERTY_INT propertyInt;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    propertyInt.DeCode(*pBitStream);

    m_pKernelModule->SetPropertyInt(propertyInt._nObjectID, propertyInt._szPropertyName, propertyInt._nValue);

    return 0;
}

int NFCClientLoginNetModule::OnObjectPropertyFloatProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_PROPERTY_FLOAT propertyFloat;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    propertyFloat.DeCode(*pBitStream);

    m_pKernelModule->SetPropertyFloat(propertyFloat._nObjectID, propertyFloat._szPropertyName, propertyFloat._fValue);

    return 0;
}

int NFCClientLoginNetModule::OnObjectPropertyDoubleProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_PROPERTY_DOUBLE propertyDouble;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    propertyDouble.DeCode(*pBitStream);

    m_pKernelModule->SetPropertyDouble(propertyDouble._nObjectID, propertyDouble._szPropertyName, propertyDouble._dwValue);

    return 0;
}

int NFCClientLoginNetModule::OnObjectPropertyStringProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_PROPERTY_STRING propertyString;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    propertyString.DeCode(*pBitStream);

    m_pKernelModule->SetPropertyString(propertyString._nObjectID, propertyString._szPropertyName, propertyString._szPropertyValue);

    return 0;
}

int NFCClientLoginNetModule::OnObjectPropertyObjectProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_PROPERTY_OBJECT propertyObject;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    propertyObject.DeCode(*pBitStream);

    m_pKernelModule->SetPropertyObject(propertyObject._nObjectID, propertyObject._szPropertyName, propertyObject._nObjectValue);

    return 0;
}

int NFCClientLoginNetModule::OnSelectServerEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (4 != var.GetCount())
    {
        return -1;
    }

    //selectServerBase._nServerID << ident;
    int nWorldID = var.IntVal(0);
    const char* pstrSsenderAddress = var.StringVal(1);
    int nLoginID = var.IntVal(2);
    const char* pstrAccount = var.StringVal(3);

    _tagLTW_WANTTO_CONNECT_WORLD selectServerBase;
    selectServerBase._nWorldID = nWorldID;
    selectServerBase._nLoginID = nLoginID;
    strcpy_s(selectServerBase._szSenderAddress, pstrSsenderAddress);
    strcpy_s(selectServerBase._szAccount, pstrAccount);

    RakNet::BitStream bitStream;
    selectServerBase.EnCode(bitStream);

    m_pThis->SendBitStream(&bitStream);

    return 0;
}

bool NFCClientLoginNetModule::AfterInit()
{
    m_pKernelModule->CreateContainer(mnClientConnectContainer, "");
    return true;
}
