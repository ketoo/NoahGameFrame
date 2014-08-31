// -------------------------------------------------------------------------
//    @FileName      :    NFCGameLogicModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameLogicModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCGameLogicModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

bool NFCGameLogicModule::Init()
{
    mnAreaID = 0;
    mnGSContainerID = -1;
    return true;
}

bool NFCGameLogicModule::Shut()
{

    return true;
}

bool NFCGameLogicModule::Execute( const float fLasFrametime, const float fStartedTime )
{
#ifdef _DEBUG
    /*
    char szContent[MAX_PATH] = { 0 };
    if (kbhit() && gets(szContent))
    {
        NFCDataList val(szContent, ",");
        if (val.GetCount() > 0)
        {
            //const char* pstrCmd = val.StringVal( 0 );
            m_pKernelModule->Command(val);

        }
    }
    */
#endif


    return true;
}

NFINT16 NFCGameLogicModule::GetAreaID()
{
    return mnAreaID;
}

bool NFCGameLogicModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    //m_pNoSqlModule = dynamic_cast<NFIDataNoSqlModule*>(pPluginManager->FindModule("NFCDataNoSqlModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>( pPluginManager->FindModule( "NFCLogicClassModule" ) );
    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    //assert(NULL != m_pNoSqlModule);
    assert( NULL != m_pLogicClassModule );


    m_pKernelModule->CreateContainer(mnGSContainerID, "");
    NFIObject* pObject = m_pKernelModule->CreateObject(0, mnGSContainerID, 0, "GameServer", "", NFCDataList());
    if (pObject)
    {
        mIdent = pObject->Self();
    }

    //m_pKernelModule->AddHeartBeat(mIdent, "TestGlobHB", TestGlobHB, NFCDataList(), 5.0f, 100000);
    return true;
}

bool NFCGameLogicModule::BeforeShut()
{

    return true;
}
