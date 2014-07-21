// -------------------------------------------------------------------------
//    @FileName      :    NFCLevelModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-28
//    @Module           :    NFCLevelModule
//
// -------------------------------------------------------------------------

#include "NFCLevelModule.h"

bool NFCLevelModule::Init()
{


    return true;
}


bool NFCLevelModule::Shut()
{
    return true;
}

bool NFCLevelModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    //位置呢
    return true;
}

bool NFCLevelModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pLogModule = dynamic_cast<NFILogModule*>( pPluginManager->FindModule( "NFCLogModule" ) );

    

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pLogModule );

    m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCLevelModule::OnObjectClassEvent );
    m_pEventProcessModule->AddClassCallBack( "AttackNPC", this, &NFCLevelModule::OnObjectClassEvent );

    return true;
}

int NFCLevelModule::AddExp( const NFIDENTID& self, const int nExp)
{
    int nCurExp = m_pKernelModule->QueryPropertyInt( self, "EXP" );
    int nLevel = m_pKernelModule->QueryPropertyInt( self, "Level" );
    int nMaxExp = 10;

    nCurExp += nExp;

    int nRemainExp = nCurExp - nMaxExp;
    while ( nRemainExp >= 0 )
    {
        //升级
        nLevel++;
        //防止越级BUG
        m_pKernelModule->SetPropertyInt( self, "Level", nLevel );

        nCurExp = nRemainExp;

        nMaxExp = 10;
        if (nMaxExp <= 0)
        {
            break;
        }

        nRemainExp -= nMaxExp;
    }

    m_pKernelModule->SetPropertyInt( self, "EXP", nCurExp );
    
    return 0;
}

int NFCLevelModule::OnKillObject( const NFIDENTID& self, const NFIDENTID& other )
{
    return 0;
}

int NFCLevelModule::OnDead( const NFIDENTID& self, const NFIDENTID& other )
{
    //降级掉经验？
    return 0;
}

int NFCLevelModule::OnObjectExpEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{

    return 0;
}

int NFCLevelModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
{
    if ( strClassName == "Player"
         && CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {
        //在事件1的时候，可能没广播出去(因为广播对象出现也是在事件1)
        m_pKernelModule->AddPropertyCallBack( self, "Level", this, &NFCLevelModule::OnObjectLevelEvent );
    }

    if ( strClassName == "AttackNPC"
         && CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {
        m_pEventProcessModule->AddEventCallBack( self, NFED_ON_OBJECT_BE_KILLED, this, &NFCLevelModule::OnObjectBeKilled );
    }

    return 0;
}

int NFCLevelModule::OnObjectBeKilled( const NFIDENTID& object, const int nEventID, const NFIValueList& var )
{
    if ( var.GetCount() == 1 && var.Type( 0 ) == TDATA_OBJECT )
    {
        NFIDENTID identKiller = var.ObjectVal( 0 );
        if ( m_pKernelModule->GetObject( identKiller ) )
        {
            int nExp = m_pKernelModule->QueryPropertyInt( object, "EXP" );
            AddExp( identKiller, nExp);
            // TODO:加怪物掉落金钱
            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, identKiller, "Add Exp for kill monster", nExp);
        }
        else
        {
            m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, identKiller, "There is no object", __FUNCTION__, __LINE__);
        }
    }

    return 0;
}

int NFCLevelModule::OnObjectLevelEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    //得到等级后，设置新的MAXEXP

    int nLevel = newVar.IntVal( 0 );
    int nMaxExp = 10;
    m_pKernelModule->SetPropertyInt( self, "MAXEXP", nMaxExp );

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "Object Level Up", nLevel);

    return 0;
}
