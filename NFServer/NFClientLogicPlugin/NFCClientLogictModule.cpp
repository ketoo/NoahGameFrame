// -------------------------------------------------------------------------
//    @FileName      :    NFCClientLogictModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCClientLogictModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include <conio.h>
#include "NFCClientLogictModule.h"
#include "NFClientLogicPlugin.h"

extern NFIPluginManager* pPluginManager;

NFIKernelModule* NFCClientLogictModule::m_pKernelModule = NULL;
NFIEventProcessModule* NFCClientLogictModule::m_pEventProcessModule = NULL;
NFIClientNetModule* NFCClientLogictModule::m_pClientNetModule = NULL;
NFCClientLogictModule* NFCClientLogictModule::m_pThis = NULL;

bool NFCClientLogictModule::Init()
{
    m_pThis = this;
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    //m_pClientNetModule = dynamic_cast<NFIClientNetModule*>( pPluginManager->FindModule( "NFCClientLoginNetModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    //assert( NULL != m_pClientNetModule );
    assert(NULL != m_pEventProcessModule);

    //m_pEventProcessModule->AddEventCallBack( 0, NFED_ON_CLIENT_DISCONNECT, OnDisconnectEvent );
    //m_pEventProcessModule->AddEventCallBack( 0, NFED_ON_CLIENT_REQUIRE_SERVER_LIST, OnRequireServerListEvent );

    return true;
}

bool NFCClientLogictModule::Shut()
{
    return true;
}

bool NFCClientLogictModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    //////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
    char szContent[MAX_PATH] = { 0 };
    if (kbhit() && gets(szContent))
    {
        NFCValueList val(szContent, ",");
        if (val.GetCount() > 0)
        {
            const char* pstrCmd = val.StringVal(0);
            if (0 == strcmp("login", pstrCmd))
            {
                if (3 == val.GetCount())
                {
                    NFCValueList valList;
                    valList << "" << val.StringVal(1) << val.StringVal(2);
                    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_LOGIN, valList);
                }
            }
            //////////////////////////////////////////////////////////////////////////
            else if (0 == strcmp("disconnect", pstrCmd))
            {
                if (2 == val.GetCount())
                {
                    const char* pstrAccount = val.StringVal(1);
                    NFCValueList valList;
                    valList << "" << pstrAccount;
                    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_LOGOUT, valList);
                }
            }
            else if (0 == strcmp("selectWorld", pstrCmd))
            {
                if (2 == val.GetCount())
                {

                    const char* pstrWorldID = val.StringVal(1);

                    NFCValueList valList;
                    valList << atoi(pstrWorldID) << 0 << 1 << "";
                    //_nWorldID,_Ident, _nLoginID, account
                    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_SELECT_SERVER, valList);
                }
            }
            //////////////////////////////////////////////////////////////////////////
            else if (0 == strcmp("createRole", pstrCmd))
            {
                if (6 == val.GetCount())
                {
                    // 名字，性别，职业，种族，阵营
                    NFCValueList valList;
                    valList << "" << val.StringVal(1) << atoi(val.StringVal(2)) << atoi(val.StringVal(3)) << atoi(val.StringVal(4)) << atoi(val.StringVal(5));
                    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_CREATEROLE, valList);
                }

            }
            //////////////////////////////////////////////////////////////////////////
            else if (0 == strcmp("deleteRole", pstrCmd))
            {
                // 名字
                if (2 == val.GetCount())
                {
                    NFCValueList valList;
                    valList << "" << val.StringVal(1);
                    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_DELETEROLE, valList);
                }

            }
            //////////////////////////////////////////////////////////////////////////
            else if (0 == strcmp("toWorld", pstrCmd))
            {
                // 名字,场景 ID
                if (3 == val.GetCount())
                {
                    NFCValueList valList;
                    valList << "" << val.StringVal(1) << atoi(val.StringVal(2));
                    m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_SELECTROLE_ENTER, valList);
                }

            }
            //////////////////////////////////////////////////////////////////////////
            else if (0 == strcmp("outWorld", pstrCmd))
            {
                NFIDENTID ident;
                ident.nSerial = atoi(val.StringVal(1));
                ident.nIdent = atoi(val.StringVal(2));


                NFCValueList valList;
                valList << "" << ident;
                m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_LEAVE_GAME, valList);
            }
            //////////////////////////////////////////////////////////////////////////
            else if (0 == strcmp("moveTo", pstrCmd))
            {

            }
            //////////////////////////////////////////////////////////////////////////
            else if (0 == strcmp("useSkillToPos", pstrCmd))
            {

            }
            //////////////////////////////////////////////////////////////////////////
            else if (0 == strcmp("useSkillToTarget", pstrCmd))
            {

            }
            else
            {
                m_pKernelModule->Command(val);
            }
        }
    }
#endif

    return true;
}

