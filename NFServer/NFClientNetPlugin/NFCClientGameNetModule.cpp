// -------------------------------------------------------------------------
//    @FileName      :    NFCClientGameNetModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCClientGameNetModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCClientGameNetModule.h"
#include "NFClientNetPlugin.h"

extern NFIPluginManager* pPluginManager;

int NFCClientGameNetModule::mnGameConnectContainer = -1;
int NFCClientGameNetModule::mnGameRoleHallContainer = -3;

NFIKernelModule* NFCClientGameNetModule::m_pKernelModule = NULL;
NFIEventProcessModule* NFCClientGameNetModule::m_pEventProcessModule = NULL;
NFCClientGameNetModule* NFCClientGameNetModule::m_pThis = NULL;

NFCClientGameNetModule::NFCClientGameNetModule()
{

}

NFCClientGameNetModule::~NFCClientGameNetModule()
{

}

bool NFCClientGameNetModule::Init()
{
    m_pThis = this;
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pEventProcessModule);

    //Initialization( false, "NFClientNetPlugin.cfg" );

    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_SELECT_SERVER_RESULTS, OnSelectServerResultsEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_CREATEROLE, OnCreateRoleEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_SELECTROLE_ENTER, OnToWorldEvent);
    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_LEAVE_GAME, OnLeaveEvent);
    //  m_pEventProcessModule->AddEventCallBack( 0, NFED_ON_CLIENT_LOGOUT, OnLogOutEvent );
    //  m_pEventProcessModule->AddEventCallBack( 0, NFED_ON_CLIENT_DISCONNECT, OnDisconnectEvent );
    //  m_pEventProcessModule->AddEventCallBack( 0, NFED_ON_CLIENT_REQUIRE_SERVER_LIST, OnRequireServerListEvent );

    return true;
}

bool NFCClientGameNetModule::Shut()
{
    Final();

    return true;
}

bool NFCClientGameNetModule::DispatcherSystemEvents(RakNet::MessageID ucEventID, RakNet::SystemAddress sAddress)
{
    switch (ucEventID)
    {
        case ID_CONNECTION_REQUEST_ACCEPTED:
        {
            printf("NFClient_Net connect successed %s\n", sAddress.ToString());
            //连接游戏服务器成功,发送密钥验证，否则5秒后被会T掉
            //客户端是否创建对象呢还是服务器创建好映射
            SendConnectKey();
        }
        break;
        case ID_CONNECTION_LOST:
        {
            printf("Client  close %s\n", sAddress.ToString());
        }
        break;
        case ID_DISCONNECTION_NOTIFICATION:
        {
            printf("NFClient_Net disconnected %s\n", sAddress.ToString());
        }
        break;
        default:
            break;
    }
    return true;
}

bool NFCClientGameNetModule::DispatcherPacket(NFIPacket* pMsgPacket)
{
    unsigned int unMsgID = pMsgPacket->GetMessageId();
    switch (unMsgID)
    {
            //  case KERNEL_MSG_CONTAIN_SPRING:
            //  case KERNEL_MSG_CONTAIN_ENDSPRING:
            //      OnContainProcess( pMsgPacket );
            //      break;

        case KERNEL_MSG_OBJECT_SPRING:
            OnObjectSpringProcess(pMsgPacket);
            break;

        case KERNEL_MSG_OBJECT_ENDSPRING:
            OnObjectEndSpringProcess(pMsgPacket);
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

        case KERNEL_MSG_GENERAL:
            OnGeneralProcess(pMsgPacket);
            break;

        case KERNEL_MSG_TABLE_ADD_ROW:
            OnObjectTableAddProcess(pMsgPacket);
            break;

        case KERNEL_MSG_TABLE_REMOVE_ROW:    //表变化
            OnObjectTableRemoveProcess(pMsgPacket);
            break;

        case KERNEL_MSG_TABLE_INT:                   //表变化
            OnObjectTableIntProcess(pMsgPacket);
            break;

        case KERNEL_MSG_TABLE_FLOAT:         //表变化
            OnObjectTableFloatProcess(pMsgPacket);
            break;

        case KERNEL_MSG_TABLE_DOUBLE:            //表变化
            OnObjectTableDoubleProcess(pMsgPacket);
            break;

        case KERNEL_MSG_TABLE_STRING:            //表变化
            OnObjectTableStringProcess(pMsgPacket);
            break;

        case KERNEL_MSG_TABLE_OBJECT:            //表变化
            OnObjectTableObjectProcess(pMsgPacket);
            break;

            //////////////////////////////////////////////////////////////////////////
        case GTC_SELECTROLE_ENTER_RESULTS:
            OnSelectRoleResultsProcess(pMsgPacket);
            break;

        default:
            printf("NFNet || 非法消息:unMsgID=%d\n", pMsgPacket->GetMessageId());
            break;
    }

    return true;
}

bool NFCClientGameNetModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    Update(fLasFrametime, fStartedTime);

    return true;
}

int NFCClientGameNetModule::OnObjectSpringProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_OBJECT_SPRING objectSpring;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    objectSpring.DeCode(*pBitStream);

    if (!m_pKernelModule->ExistContainer(objectSpring._nContainer))
    {
        m_pKernelModule->CreateContainer(objectSpring._nContainer, "");
    }

    m_pKernelModule->CreateObject(objectSpring._nObjectID, objectSpring._nContainer, objectSpring._nGroupID, objectSpring._szClassName, objectSpring._szConfigName, NFCValueList());

    return 0;
}

int NFCClientGameNetModule::OnObjectPropertyIntProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_PROPERTY_INT propertyInt;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    propertyInt.DeCode(*pBitStream);

    m_pKernelModule->SetPropertyInt(propertyInt._nObjectID, propertyInt._szPropertyName, propertyInt._nValue);

    return 0;
}

int NFCClientGameNetModule::OnObjectPropertyFloatProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_PROPERTY_FLOAT propertyFloat;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    propertyFloat.DeCode(*pBitStream);

    m_pKernelModule->SetPropertyFloat(propertyFloat._nObjectID, propertyFloat._szPropertyName, propertyFloat._fValue);

    return 0;
}

int NFCClientGameNetModule::OnObjectPropertyDoubleProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_PROPERTY_DOUBLE propertyDouble;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    propertyDouble.DeCode(*pBitStream);

    m_pKernelModule->SetPropertyDouble(propertyDouble._nObjectID, propertyDouble._szPropertyName, propertyDouble._dwValue);

    return 0;
}

int NFCClientGameNetModule::OnObjectPropertyStringProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_PROPERTY_STRING propertyString;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    propertyString.DeCode(*pBitStream);

    m_pKernelModule->SetPropertyString(propertyString._nObjectID, propertyString._szPropertyName, propertyString._szPropertyValue);

    return 0;
}

int NFCClientGameNetModule::OnObjectPropertyObjectProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_PROPERTY_OBJECT propertyObject;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    propertyObject.DeCode(*pBitStream);

    m_pKernelModule->SetPropertyObject(propertyObject._nObjectID, propertyObject._szPropertyName, propertyObject._nObjectValue);

    return 0;
}

int NFCClientGameNetModule::OnContainProcess(NFIPacket* pMsgPacket)
{
    return 0;
}

int NFCClientGameNetModule::OnSelectServerResultsEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (7 != var.GetCount())
    {
        return -1;
    }

    //_nWorldID,_Ident, _szWorldAddress, _szWorldKey
    int nWorldID = var.IntVal(0);
    NFIDENTID ident = var.ObjectVal(1);
    int nLoginID = var.IntVal(2);

    const char* pstrAccount = var.StringVal(3);
    const char* pstrWorldAddress = var.StringVal(4);
    int nPort = var.IntVal(5);
    const char* pstrWorldKey = var.StringVal(6);

    m_pThis->mstrAccount = pstrAccount;
    m_pThis->mstrWorldAddress = pstrWorldAddress;
    m_pThis->mnPort = nPort;
    m_pThis->mstrWorldKey = pstrWorldKey;

    //连接GameServer
    m_pThis->Initialization(pstrWorldAddress, nPort, "shadow");

    return 0;
}

bool NFCClientGameNetModule::AfterInit()
{
    m_pKernelModule->CreateContainer(mnGameConnectContainer, "");
    m_pKernelModule->CreateContainer(mnGameRoleHallContainer, "");
    //m_pKernelModule->CreateContainer( 1, "" );

    return true;
}

void NFCClientGameNetModule::SendConnectKey()
{
    _tagCTG_CONNECT_KEY sendKey;
    strcpy_s(sendKey._szAccount, mstrAccount.c_str());
    strcpy_s(sendKey._szKey, mstrWorldKey.c_str());

    RakNet::BitStream bitStream;
    sendKey.EnCode(bitStream);

    SendBitStream(&bitStream);
}

int NFCClientGameNetModule::OnObjectTableIntProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_TABLE_INT recordInt;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    recordInt.DeCode(*pBitStream);

    m_pKernelModule->SetRecordInt(recordInt._nObjectID, recordInt._szTableName, recordInt._nRow, recordInt._nCol, recordInt._nValue);

    return 0;
}

int NFCClientGameNetModule::OnObjectTableFloatProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_TABLE_FLOAT recordFloat;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    recordFloat.DeCode(*pBitStream);

    m_pKernelModule->SetRecordFloat(recordFloat._nObjectID, recordFloat._szTableName, recordFloat._nRow, recordFloat._nCol, recordFloat._fValue);

    return 0;
}

int NFCClientGameNetModule::OnObjectTableDoubleProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_TABLE_DOUBLE recordDouble;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    recordDouble.DeCode(*pBitStream);

    m_pKernelModule->SetRecordDouble(recordDouble._nObjectID, recordDouble._szTableName, recordDouble._nRow, recordDouble._nCol, recordDouble._dwValue);

    return 0;
}

int NFCClientGameNetModule::OnObjectTableStringProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_TABLE_STRING recordString;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    recordString.DeCode(*pBitStream);

    m_pKernelModule->SetRecordString(recordString._nObjectID, recordString._szTableName, recordString._nRow, recordString._nCol, recordString._szValue);

    return 0;
}

int NFCClientGameNetModule::OnObjectTableObjectProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_TABLE_OBJECT recordObject;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    recordObject.DeCode(*pBitStream);

    m_pKernelModule->SetRecordObject(recordObject._nObjectID, recordObject._szTableName, recordObject._nRow, recordObject._nCol, recordObject._nObjectValue);

    return 0;
}

int NFCClientGameNetModule::OnCreateRoleEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (6 != var.GetCount())
    {
        return 0;
    }

    //const char* pstrRoleName = var.StringVal(0);
    const char* pstrRoleName = var.StringVal(1);
    int nRoleSex = var.IntVal(2);
    int nRoleJob = var.IntVal(3);
    int nRoleRace = var.IntVal(4);
    int nRoleCamp = var.IntVal(5);

    RakNet::BitStream oBitStream;
    _tagCTG_CREATE_ROLE createRole;

    createRole._nSex = nRoleSex;
    createRole._nJob = nRoleJob;
    createRole._nRace = nRoleRace;
    createRole._nCamp = nRoleCamp;
    strcpy(createRole._szName, pstrRoleName);


    createRole.EnCode(oBitStream);

    m_pThis->SendBitStream(&oBitStream);

    return 0;
}

int NFCClientGameNetModule::OnDeleteRoleEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (2 != var.GetCount())
    {
        return 0;
    }

    //const char* pstrRoleName = var.StringVal(0);
    const char* pstrRoleName = var.StringVal(1);

    RakNet::BitStream oBitStream;
    _tagCTG_DELETE_ROLE deleteRole;

    strcpy(deleteRole._szName, pstrRoleName);

    deleteRole.EnCode(oBitStream);

    m_pThis->SendBitStream(&oBitStream);

    return 0;
}

int NFCClientGameNetModule::OnToWorldEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (3 != var.GetCount())
    {
        return 0;
    }
    //const char* pstrRoleName = var.StringVal(0);
    const char* pstrRoleName = var.StringVal(1);
    int nSceneIndex = var.IntVal(2);

    RakNet::BitStream oBitStream;
    _tagCTG_ENTER_WORLD enterWorld;

    strcpy(enterWorld._szName, pstrRoleName);
    enterWorld._nSceneIndex = nSceneIndex;

    enterWorld.EnCode(oBitStream);
    m_pThis->SendBitStream(&oBitStream);

    return 0;
}

int NFCClientGameNetModule::OnSelectRoleResultsProcess(NFIPacket* pMsgPacket)
{
    _tagGTC_SELECTROLE_ENTER_RESULTS results;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();

    results.DeCode(*pBitStream);

    const char* pstrGSAddress = results._szAddress;

    NFCValueList var;
    var.Split(pstrGSAddress, "|");
    if (2 == var.GetCount())
    {
        //重新连接GS
        m_pThis->Initialization(var.StringVal(0), atoi(var.StringVal(1)), "shadow");
    }

    return 0;
}

int NFCClientGameNetModule::OnObjectEndSpringProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_OBJECT_ENDSPRING objectEndSpring;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    objectEndSpring.DeCode(*pBitStream);

    m_pKernelModule->DestroyObject(objectEndSpring._nObjectID);

    return 0;
}

int NFCClientGameNetModule::OnLeaveEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (2 != var.GetCount())
    {
        return 0;
    }

    RakNet::BitStream oBitStream;
    _tagCTG_LEAVE_WORLD leaveWorld;

    NFIDENTID ident = var.ObjectVal(1);
    leaveWorld._Ident = ident.nIdent;

    leaveWorld.EnCode(oBitStream);
    m_pThis->SendBitStream(&oBitStream);

    return 0;
}

int NFCClientGameNetModule::OnGeneralProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_GENERAL generalMsg;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    generalMsg.DeCode(*pBitStream);

    switch (generalMsg._nSubMsgID)
    {

        default:
            break;
    }

    return 0;
}

int NFCClientGameNetModule::OnObjectTableAddProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_TABLE_ADD_ROW recordAdd;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    recordAdd.DeCode(*pBitStream);

    NFIObject* pObject = m_pKernelModule->GetObject(recordAdd._nObjectID);
    if (pObject)
    {
        NFIRecord* pRecord = pObject->GetRecordManager()->GetElement(recordAdd._szTableName);
        if (pRecord)
        {
            pRecord->AddRow(recordAdd._nRow, *recordAdd._pInfo);
        }
    }

    return 0;
}

int NFCClientGameNetModule::OnObjectTableRemoveProcess(NFIPacket* pMsgPacket)
{
    _tagKERNEL_MSG_TABLE_REMOVE_ROW recordRemove;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    recordRemove.DeCode(*pBitStream);

    NFIObject* pObject = m_pKernelModule->GetObject(recordRemove._nObjectID);
    if (pObject)
    {
        NFIRecord* pRecord = pObject->GetRecordManager()->GetElement(recordRemove._szTableName);
        if (pRecord)
        {
            pRecord->Remove(recordRemove._nRow);
        }
    }

    return 0;
}
