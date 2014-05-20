// -------------------------------------------------------------------------
//    @FileName      :    NFCDataProcessModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             ：    2013-10-03
//    @Module           :    NFCDataProcessModule
//
// -------------------------------------------------------------------------


#include "NFCDataProcessModule.h"
#include "NFComm\NFMessageDefine\NFDefine.pb.h"

bool NFCDataProcessModule::Init()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pNoSqlModule = dynamic_cast<NFIDataNoSqlModule*>( pPluginManager->FindModule( "NFCDataNoSqlModule" ) );
    m_pGameLogicModule = dynamic_cast<NFIGameLogicModule*>( pPluginManager->FindModule( "NFCGameLogicModule" ) );
    

    m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCDataProcessModule::OnObjectClassEvent );

    return true;
}


bool NFCDataProcessModule::Shut()
{
    return true;
}

bool NFCDataProcessModule::Execute( const float fLasFrametime, const float fStartedTime )
{

    return true;
}

bool NFCDataProcessModule::AfterInit()
{


    return true;
}

int NFCDataProcessModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
{
    //非完整NF架构目前不使用NOSQL
    return 0;

    if ( strClassName == "Player" )
    {
        if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
        {
            SaveDataToNoSql( self );
        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent )
        {
            LoadDataFormNoSql( self );
        }
    }

    return 0;
}

int NFCDataProcessModule::LoadDataFormNoSql( const NFIDENTID& self )
{
    NFIObject* pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        std::string strRoleName = pObject->QueryPropertyString( "RoleName" );
        NFIPropertyManager* pProManager = pObject->GetPropertyManager();
        if ( pProManager )
        {
            //LoadProperty( strRoleName, pProManager );
        }

        NFIRecordManager* pRecordManager = pObject->GetRecordManager();
        if ( pRecordManager )
        {
            std::string strRecordName;
            NFIRecord* pRecord = pRecordManager->First( strRecordName );
            while ( pRecord )
            {
                //LoadRecord( strRoleName, strRecordName, pRecord );

                pRecord = pRecordManager->Next( strRecordName );
            }
        }
        else
        {
            //否则只载入外观装备view表
            NFIRecord* pRecord = pRecordManager->GetElement( "PlayerViewItem" );
            if ( pRecord )
            {
                //LoadRecord( strRoleName, "PlayerViewItem", pRecord );
            }
        }
    }

    return 0;
}

int NFCDataProcessModule::SaveDataToNoSql(const NFIDENTID& self, bool bOffline/* = false*/)
{
    NFIObject* pObject = m_pKernelModule->GetObject( self );
    if ( pObject )
    {
        const std::string& strRoleName = pObject->QueryPropertyString( "RoleName" );
        NFIPropertyManager* pProManager = pObject->GetPropertyManager();
        if ( pProManager )
        {
            //SaveProperty( strRoleName, pProManager );
        }

        //////////////////////////////////////////////////////////////////////////
        NFIRecordManager* pRecordManager = pObject->GetRecordManager();
        std::string strRecordName;
        NFIRecord* pRecord = pRecordManager->First( strRecordName );
        while ( pRecord )
        {
            //表数据
            if ( pRecord->GetSave() )
            {
                //SaveRecord( strRoleName, strRecordName, pRecord );
            }

            pRecord = pRecordManager->Next( strRecordName );
        }
    }

    return 0;
}

void NFCDataProcessModule::Handler( const NFIActorMessage& message, const Theron::Address from )
{
    //收到的消息协议
    if (message.eType == NFIActorMessage::EACTOR_NET_MSG)
    {
        HandlerEx(message, from);
        //消息来了
    }
    else if (message.eType == NFIActorMessage::EACTOR_TRANS_MSG)
    {
        //想转移actor,其实是切场景的时候，玩家当前actor无那个场景,所以才切换actor
        HandlerTrans(message, from);
    }
    else if (message.eType == NFIActorMessage::EACTOR_LOG_MSG)
    {
        HandlerLog(message, from);
    }
}

Theron::Address NFCDataProcessModule::GetActorID( const NFIDENTID& self )
{
    return Theron::Address();
}

void NFCDataProcessModule::HandlerEx( const NFIActorMessage& message, const Theron::Address from )
{
    NFMsg::EGameMsgID eMsgID = (NFMsg::EGameMsgID)message.nSubMsgID;
    //真的具体的业务
    //case eMsgID


}

void NFCDataProcessModule::HandlerTrans( const NFIActorMessage& message, const Theron::Address from )
{
    //别人想进入这个场景
}

void NFCDataProcessModule::HandlerLog( const NFIActorMessage& message, const Theron::Address from )
{
    //日志
}
