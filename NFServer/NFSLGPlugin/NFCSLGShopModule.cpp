// -------------------------------------------------------------------------
//    @FileName      :    NFCSLGShopModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2014-10-05
//    @Module           :    NFCSLGShopModule
//
// -------------------------------------------------------------------------

#include "NFCSLGShopModule.h"

bool NFCSLGShopModule::Init()
{


    return true;
}


bool NFCSLGShopModule::Shut()
{
    return true;
}

bool NFCSLGShopModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    //λ����
    return true;
}

bool NFCSLGShopModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pLogModule = dynamic_cast<NFILogModule*>( pPluginManager->FindModule( "NFCLogModule" ) );

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pLogModule );


    return true;
}

bool NFCSLGShopModule::OnReqBuyItem(const NFIDENTID& self, const std::string& strID, const int nCount, const float fX, const float fY, const float fZ )
{
	//�۳�����
	//��ӽ���
	//

	return true;
}
