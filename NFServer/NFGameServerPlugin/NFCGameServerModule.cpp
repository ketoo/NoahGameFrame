// -------------------------------------------------------------------------
//    @FileName			:    NFCGameServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCGameServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFGameServerPlugin.h"
#include "NFCGameServerModule.h"


bool NFCGameServerModule::Init()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
    return true;
}

bool NFCGameServerModule::Shut()
{

    return true;
}

bool NFCGameServerModule::Execute()
{
#ifdef _DEBUG
    /*
    char szContent[MAX_PATH] = { 0 };
    if (kbhit() && gets(szContent))
    {
        NFDataList val(szContent, ",");
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

bool NFCGameServerModule::AfterInit()
{


    return true;
}

bool NFCGameServerModule::BeforeShut()
{

    return true;
}
