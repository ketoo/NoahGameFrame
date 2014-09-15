// -------------------------------------------------------------------------
//    @FileName      :    NFCSLGModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCSLGModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFSLGPlugin.h"
#include "NFCSLGModule.h"
#include "NFComm/Config/NFConfig.h"

bool NFCSLGModule::Init()
{
    return true;
}

bool NFCSLGModule::Shut()
{

    return true;
}

bool NFCSLGModule::Execute( const float fLasFrametime, const float fStartedTime )
{
#ifdef _DEBUG
    /*
    char szContent[MAX_PATH] = { 0 };
    if (kbhit() && gets(szContent))
    {
        NFCDataList val(szContent, ",");
        if (val.GetCount() > 0)
        {
            //const char* pstrCmd = val.String( 0 );
            m_pKernelModule->Command(val);

        }
    }
    */
#endif


    return true;
}

bool NFCSLGModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>( pPluginManager->FindModule( "NFCLogicClassModule" ) );

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pLogicClassModule );

    return true;
}

bool NFCSLGModule::BeforeShut()
{

    return true;
}
