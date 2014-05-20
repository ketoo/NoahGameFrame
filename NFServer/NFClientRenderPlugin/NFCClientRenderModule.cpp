// -------------------------------------------------------------------------
//    @FileName      :    NFCClientRenderModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCClientRenderModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCClientRenderModule.h"
#include "NFClientRenderPlugin.h"

extern NFIPluginManager* pPluginManager;

NFIEventProcessModule* NFCClientRenderModule::m_pEventProcessModule = NULL;
NFCClientRenderModule* NFCClientRenderModule::m_pThis = NULL;

bool NFCClientRenderModule::Init()
{
    m_pThis = this;
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));

    assert(NULL != m_pEventProcessModule);
    //m_pEventProcessModule->AddEventCallBack( 0, NFED_ON_CLIENT_SELECT_SERVER, OnSelectServerEvent );

    return true;
}

bool NFCClientRenderModule::Shut()
{
    return true;
}

bool NFCClientRenderModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

// bool NFCClientRenderModule::RegisterFunction( char* strFuncName, void* pFunc )
// {
//     FunctionPointMap::iterator it = mtFuncPoint.find( strFuncName );
//     if ( it != mtFuncPoint.end() )
//     {
//         mtFuncPoint.erase( it );
//     }
//
//     mtFuncPoint.insert( FunctionPointMap::value_type( strFuncName, pFunc ) );
//
//     return true;
// }

bool NFCClientRenderModule::AfterInit()
{
    //     RegisterFunction( "OnObjectProcess", NULL );
    //
    //     RegisterFunction( "OnObjectPropertyIntProcess", NULL );
    //     RegisterFunction( "OnObjectPropertyFloatProcess", NULL );
    //     RegisterFunction( "OnObjectPropertyDoubleProcess", NULL );
    //     RegisterFunction( "OnObjectPropertyStringProcess", NULL );
    //     RegisterFunction( "OnObjectPropertyObjectProcess", NULL );
    //
    //     RegisterFunction( "OnObjectTableIntProcess", NULL );
    //     RegisterFunction( "OnObjectTableFloatProcess", NULL );
    //     RegisterFunction( "OnObjectTableDoubleProcess", NULL );
    //     RegisterFunction( "OnObjectTableStringProcess", NULL );
    //     RegisterFunction( "OnObjectTableObjectProcess", NULL );
    //
    //     //控制行走,技能的函数
    //     RegisterFunction( "OnObjectMoveProcess", NULL );
    //     RegisterFunction( "OnObjectSkillTargetProcess", NULL );
    //     RegisterFunction( "OnObjectSkillPosProcess", NULL );

    return true;
}