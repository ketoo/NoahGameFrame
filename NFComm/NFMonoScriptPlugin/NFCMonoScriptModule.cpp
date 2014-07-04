// -------------------------------------------------------------------------
//    @FileName      :    NFCMonoScriptModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCMonoScriptModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCMonoScriptModule.h"
#include "NFMonoScriptPlugin.h"
#include "mono\metadata\metadata.h"
#include "mono\jit\jit.h"
#include "mono\metadata\assembly.h"

bool NFCMonoScriptModule::Init()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pLogicClassModule);


    return true;
}

bool NFCMonoScriptModule::AfterInit()
{
    //mono_set_assemblies_path("E:\\Projects\\MonoTest\\MonoTest\\");
    //MonoDomain* domain = mono_jit_init("ScriptDomain");

    return true;
}

bool NFCMonoScriptModule::Shut()
{

    return true;
}

bool NFCMonoScriptModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

int NFCMonoScriptModule::DoScript( const NFIDENTID& self, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCMonoScriptModule::DoEventScript( const NFIDENTID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCMonoScriptModule::DoHeartBeatScript( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCMonoScriptModule::DoScriptPropertyCallBack( const NFIDENTID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCScriptVarList& oldVar, const NFCScriptVarList& neVar, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCMonoScriptModule::DoScriptRecordCallBack( const NFIDENTID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCScriptVarList& oldVar, const NFCScriptVarList& newVar, const NFCScriptVarList& arg )
{
    return 0;
}

int NFCMonoScriptModule::DoHeartBeatCommonCB( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var )
{
    return 0;
}

int NFCMonoScriptModule::DoEventCommonCB( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    return 0;
}
