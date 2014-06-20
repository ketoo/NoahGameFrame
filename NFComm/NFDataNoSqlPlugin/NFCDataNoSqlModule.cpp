// -------------------------------------------------------------------------
//    @FileName      :    NFCDataNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataNoSqlModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include <algorithm>
#include "NFCDataNoSqlModule.h"

NFCDataNoSqlModule::NFCDataNoSqlModule(NFIPluginManager* p)
{
    pPluginManager = p;
}

NFCDataNoSqlModule::~NFCDataNoSqlModule()
{

}

bool NFCDataNoSqlModule::Init()
{
    m_pPlayerDataDriver = new NFCDataNoSqlDriver();
	m_pPlayerDataDriver->Connect("127.0.0.1");

    return true;
}

bool NFCDataNoSqlModule::Shut()
{
    if (m_pPlayerDataDriver)
    {
        delete m_pPlayerDataDriver;
        m_pPlayerDataDriver = NULL;
    }

    return true;
}


bool NFCDataNoSqlModule::AfterInit()
{


    return true;
}

bool NFCDataNoSqlModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

