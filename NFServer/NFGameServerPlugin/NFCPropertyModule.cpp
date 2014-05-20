// -------------------------------------------------------------------------
//    @FileName         :    NFCPropertyModule.cpp
//    @Author               :    LvSheng.Huang
//    @Date                 :    2013-07-05
//    @Module               :    NFCPropertyModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCPropertyModule.h"
#include "NFComm/NFCore/NFTimer.h"

bool NFCPropertyModule::Init()
{
    mstrCommPropertyName = "CommPropertyValue";

    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>( pPluginManager->FindModule( "NFCEventProcessModule" ) );
    m_pKernelModule = dynamic_cast<NFIKernelModule*>( pPluginManager->FindModule( "NFCKernelModule" ) );
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>( pPluginManager->FindModule( "NFCElementInfoModule" ) );
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>( pPluginManager->FindModule( "NFCLogicClassModule" ) );
    m_pPropertyConfigModule = dynamic_cast<NFIPropertyConfigModule*>( pPluginManager->FindModule( "NFCPropertyConfigModule" ) );
    m_pLevelConfigModule = dynamic_cast<NFILevelConfigModule*>( pPluginManager->FindModule( "NFCLevelConfigModule" ) );

    

    assert( NULL != m_pEventProcessModule );
    assert( NULL != m_pKernelModule );
    assert( NULL != m_pElementInfoModule );
    assert( NULL != m_pLogicClassModule );
    assert( NULL != m_pPropertyConfigModule );
    assert( NULL != m_pLevelConfigModule );

    m_pEventProcessModule->AddClassCallBack( "Player", this, &NFCPropertyModule::OnObjectClassEvent );
    m_pEventProcessModule->AddClassCallBack( "NPC", this, &NFCPropertyModule::OnObjectClassEvent );

    return true;
}


bool NFCPropertyModule::Shut()
{
    return true;
}

bool NFCPropertyModule::Execute( const float fLasFrametime, const float fStartedTime )
{
    return true;
}

bool NFCPropertyModule::AfterInit()
{
    //做好动态表ID和属性名的映射,得到表的描述
    NFIRecordManager* pRecordMmanager = m_pLogicClassModule->GetClassRecordManager( "Player" );
    NFIRecord* pRecord = pRecordMmanager->GetElement( mstrCommPropertyName );
    const NFIValueList& descList = pRecord->GetInitDesc();

    for ( int i = 0; i < descList.GetCount(); i++ )
    {
        const std::string& strPropertyName = descList.StringVal( i );
        int* pCol = new int( i );
        mPropertyColList.AddElement( strPropertyName, pCol );
        mColPropertyList.AddElement( *pCol, new std::string( strPropertyName ) );
    }

    return true;
}

int NFCPropertyModule::GetPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType )
{
    int* pTableCol = mPropertyColList.GetElement( strPropertyName );
    if ( pTableCol && NFPropertyGroup::NPG_ALL != eGroupType )
    {
        return m_pKernelModule->QueryRecordInt( self, mstrCommPropertyName, eGroupType, *pTableCol );
    }

    return m_pKernelModule->QueryPropertyInt( self, mstrCommPropertyName );
}

int NFCPropertyModule::GetPropertyValue( const NFIDENTID& self, const NFIPropertyConfigModule::NFRumTimeColIndex nCol, const NFPropertyGroup eGroupType )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        return m_pKernelModule->QueryRecordInt( self, mstrCommPropertyName, eGroupType, nCol );
    }
    return 0;
}

int NFCPropertyModule::SetPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue )
{
    int* pTableCol = mPropertyColList.GetElement( strPropertyName );
    if ( pTableCol && NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NFIObject* pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NFIRecord* pRecord = pObject->GetRecordManager()->GetElement(mstrCommPropertyName);
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                return pRecord->SetInt( eGroupType, *pTableCol, nValue );
            }
        }

        //return m_pKernelModule->SetRecordInt( self, mstrCommPropertyName, eGroupType, *pTableCol, nValue );
    }

    //动态表中没有，则设置到最终值
    m_pKernelModule->SetPropertyInt( self, strPropertyName, nValue );

    return 0;
}

int NFCPropertyModule::SetPropertyValue( const NFIDENTID& self, const NFIPropertyConfigModule::NFRumTimeColIndex nCol, const NFPropertyGroup eGroupType, const int nValue )
{
    if ( NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NFIObject* pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NFIRecord* pRecord = pObject->GetRecordManager()->GetElement(mstrCommPropertyName);
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                return pRecord->SetInt( eGroupType, nCol, nValue );
            }
        }
    }

    return 0;
}

int NFCPropertyModule::AddPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue )
{
    int* pTableCol = mPropertyColList.GetElement( strPropertyName );
    if ( pTableCol && NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NFIObject* pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NFIRecord* pRecord = pObject->GetRecordManager()->GetElement(mstrCommPropertyName);
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                int nPropertyValue = pRecord->QueryInt(eGroupType, *pTableCol );

                return pRecord->SetInt( eGroupType, *pTableCol, nPropertyValue + nValue );
            }
        }
    }

    return 0;
}

int NFCPropertyModule::SubPropertyValue( const NFIDENTID& self, const std::string& strPropertyName, const NFPropertyGroup eGroupType, const int nValue )
{
    int* pTableCol = mPropertyColList.GetElement( strPropertyName );
    if ( pTableCol && NFPropertyGroup::NPG_ALL != eGroupType )
    {
        NFIObject* pObject = m_pKernelModule->GetObject(self);
        if (pObject)
        {
            NFIRecord* pRecord = pObject->GetRecordManager()->GetElement(mstrCommPropertyName);
            if (pRecord)
            {
                pRecord->SetUsed(eGroupType, true);
                int nPropertyValue = pRecord->QueryInt(eGroupType, *pTableCol );

                return pRecord->SetInt( eGroupType, *pTableCol, nPropertyValue - nValue );
            }
        }
    }

    return 0;
}

int NFCPropertyModule::OnObjectPropertyEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    return 0;
}

int NFCPropertyModule::OnObjectLevelEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    RefreshBaseProperty( self );

    int nLevel = oldVar.IntVal( 0 );
    int nMaxExp = m_pLevelConfigModule->GetNeedExp( nLevel );
    m_pKernelModule->SetPropertyInt( self, "MAXEXP", nMaxExp );

    FullHPMP(self);
    FullSP(self);

    return 0;
}

int NFCPropertyModule::OnRecordPropertyEvent( const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    //计算总值
    //col 属性值枚举
    //row 分层

    int nAllValue = 0;
    NFIObject* pObject = m_pKernelModule->GetObject( self );
    NFIRecord* pRecord = pObject->GetRecordManager()->GetElement( mstrCommPropertyName );
    for ( int i = 0; i < ( int )( NFPropertyGroup::NPG_ALL ); i++ )
    {
        if ( i < pRecord->GetRows() )
        {
            int nValue = pRecord->QueryInt( i, nCol );
            nAllValue += nValue;
        }
    }

    std::string* strProperty = mColPropertyList.GetElement( nCol );
    if ( strProperty )
    {
        pObject->SetPropertyInt( ( *strProperty ).c_str(), nAllValue );
    }


    return 0;
}

int NFCPropertyModule::OnObjectClassEvent( const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var )
{
    //  for (int i  = 0; i < (int)NFPropertyColIndex::PROPERTY_MAX; i++)
    //  {
    //      m_pKernelModule->AddPropertyCallBack( self, mstrPropertyName[i], this, &NFCPropertyModule::OnObjectPropertyEvent );
    //  }
    if ( strClassName == "Player" )
    {
        if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
        {
            m_pKernelModule->AddPropertyCallBack( self, "Level", this, &NFCPropertyModule::OnObjectLevelEvent );
            m_pKernelModule->AddPropertyCallBack( self, "HP", this, &NFCPropertyModule::OnObjectHPEvent );

            // TODO:一级属性回调

            m_pKernelModule->AddRecordCallBack( self, mstrCommPropertyName, this, &NFCPropertyModule::OnRecordPropertyEvent );

            //m_pEventProcessModule->AddEventCallBack(self, EFID_ON_REQMODIFY_OBJECT_DATA, this, &NFCPropertyModule::OnReqModifyData);

        }
        else if ( CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA == eClassEvent )
        {
            int nOnlineCount = m_pKernelModule->QueryPropertyInt( self, "OnlineCount" );
            if ( nOnlineCount <= 0 && m_pKernelModule->QueryPropertyInt( self, "SceneID" ) > 0 )
            {
                //第一次出生，设置基础属性
                RefreshBaseProperty( self );
            }

            //上线后满状态
            FullHPMP( self );
            FullSP(self);
        }
        else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
        {
                //血回复，10秒回复一次(实际不需要服务器计算)
                //m_pKernelModule->AddHeartBeat(self, "OnHPRecoverHeart", this, &NFCPropertyModule::OnHPRecoverHeart, NFCValueList(), 10, 1000000);
                //蓝回复(技能石)，10秒回复一次(实际不需要服务器计算)
                //m_pKernelModule->AddHeartBeat(self, "OnMPRecoverHeart", this, &NFCPropertyModule::OnMPRecoverHeart, NFCValueList(), 10, 1000000);
                //体力回复，(实际不需要服务器计算)
                //m_pKernelModule->AddHeartBeat(self, "OnSPRecoverHeart", this, &NFCPropertyModule::OnSPRecoverHeart, NFCValueList(), 10, 1000000);
                //关卡疲劳值，15分钟回复一次
                m_pKernelModule->AddHeartBeat(self, "OnVPRecoverHeart", this, &NFCPropertyModule::OnVPRecoverHeart, NFCValueList(), 15*60, 1000000);
                //活动疲劳值，15分钟回复一次
                m_pKernelModule->AddHeartBeat(self, "OnActivityVPRecoverHeart", this, &NFCPropertyModule::OnActivityVPRecoverHeart, NFCValueList(), 15*60, 1000000);
        }
    }

    return 0;
}

int NFCPropertyModule::RefreshBaseProperty( const NFIDENTID& self )
{
    //初始属性+等级属性(职业决定)
    NFJobType eJobType = ( NFJobType )m_pKernelModule->QueryPropertyInt( self, "Job" );
    int nLevel = m_pKernelModule->QueryPropertyInt( self, "Level" );

    for ( int i = 0; i < NFIPropertyConfigModule::NFRumTimeColIndex::NFRTC_END; ++i )
    {
        std::string* strPropertyName = mColPropertyList.GetElement( i );
        int nValue = m_pPropertyConfigModule->CalculateBaseValue( eJobType, nLevel, *strPropertyName );
        SetPropertyValue( self, ( NFIPropertyConfigModule::NFRumTimeColIndex )i, NFPropertyGroup::NPG_JOBLEVEL, nValue );
    }

    return 1;
}

int NFCPropertyModule::FullHPMP( const NFIDENTID& self )
{
    int nMaxHP = m_pKernelModule->QueryPropertyInt( self, "MAXHP" );
    if ( nMaxHP > 0 )
    {
        m_pKernelModule->SetPropertyInt( self, "HP", nMaxHP );
    }

    int nMaxMP = m_pKernelModule->QueryPropertyInt( self, "MAXMP" );
    if ( nMaxMP > 0 )
    {
        m_pKernelModule->SetPropertyInt( self, "MP", nMaxMP );
    }

    return 0;
}

int NFCPropertyModule::OnObjectHPEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar )
{
    if ( newVar.IntVal( 0 ) <= 0 )
    {
        //m_pKernelModule->DestroyObject(self);
    }

    //std::cout << strPropertyName << "=======" << newVar.IntVal(0) << std::endl;

    return 0;
}

int NFCPropertyModule::AddHP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "HP" );
    int nMaxValue = m_pKernelModule->QueryPropertyInt( self, "MAXHP" );

    if ( nCurValue > 0 )
    {
        nCurValue += nValue;
        if ( nCurValue > nMaxValue )
        {
            nCurValue = nMaxValue;
        }
        m_pKernelModule->SetPropertyInt( self, "HP", nCurValue );
    }

    return nCurValue;
}

int NFCPropertyModule::AddMP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "MP" );
    int nMaxValue = m_pKernelModule->QueryPropertyInt( self, "MAXMP" );

    nCurValue += nValue;
    if ( nCurValue > nMaxValue )
    {
        nCurValue = nMaxValue;
    }
    m_pKernelModule->SetPropertyInt( self, "MP", nCurValue );

    return nCurValue;
}

int NFCPropertyModule::ConsumeHP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "HP" );
    if ( ( nCurValue > 0 ) && ( nCurValue - nValue >= 0 ) )
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt( self, "HP", nCurValue );

        return nValue;
    }

    return -1;
}

int NFCPropertyModule::ConsumeMP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "MP" );
    if ( ( nCurValue > 0 ) && ( nCurValue - nValue >= 0 ) )
    {
        nCurValue -= nValue;
        m_pKernelModule->SetPropertyInt( self, "MP", nCurValue );

        return nValue;
    }

    return 0;
}

int NFCPropertyModule::ConsumeSP( const NFIDENTID& self, int nValue )
{
    int nCSP = m_pKernelModule->QueryPropertyInt( self, "SP" );
    if ( ( nCSP > 0 ) && ( nCSP - nValue >= 0 ) )
    {
        nCSP -= nValue;
        m_pKernelModule->SetPropertyInt( self, "SP", nCSP );

        return nValue;
    }

    return -1;
}

int NFCPropertyModule::FullSP( const NFIDENTID& self )
{
    int nMAXCSP = m_pKernelModule->QueryPropertyInt( self, "MAXSP" );
    if ( nMAXCSP > 0 )
    {
        m_pKernelModule->SetPropertyInt( self, "SP", nMAXCSP );

        return 0;
    }

    return -1;
}

int NFCPropertyModule::AddSP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "SP" );
    int nMaxValue = m_pKernelModule->QueryPropertyInt( self, "MAXSP" );

    nCurValue += nValue;
    if ( nCurValue > nMaxValue )
    {
        nCurValue = nMaxValue;
    }
    m_pKernelModule->SetPropertyInt( self, "SP", nCurValue );

    return nCurValue;
}

int NFCPropertyModule::ConsumeVP( const NFIDENTID& self, int nValue )
{
    int nVP = m_pKernelModule->QueryPropertyInt( self, "VP" );
    if ( ( nVP > 0 ) && ( nVP - nValue >= 0 ) )
    {
        nVP -= nValue;
        m_pKernelModule->SetPropertyInt( self, "VP", nVP );

        return nValue;
    }

    return -1;
}

int NFCPropertyModule::FullVP( const NFIDENTID& self )
{
    int nMaxVP = m_pKernelModule->QueryPropertyInt( self, "MAXVP" );
    if ( nMaxVP > 0 )
    {
        m_pKernelModule->SetPropertyInt( self, "VP", nMaxVP );

        return 0;
    }

    return 0;
}

int NFCPropertyModule::RestoreVP( const NFIDENTID& self )
{
    NFINT64 nLastOfflineTime = m_pKernelModule->QueryPropertyObject(self, "LastOfflineTime").nData64;
    NFINT32 nOfflineTime = NFTimeEx::GetNowTime() - nLastOfflineTime;
    int nValue = (nOfflineTime / (15 * 60.0f)) * m_pKernelModule->QueryPropertyInt(self, "VPREGEN");
    AddVP(self, nValue);
    return 0;
}

int NFCPropertyModule::AddVP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "VP" );
    int nMaxValue = m_pKernelModule->QueryPropertyInt( self, "MAXVP" );

    nCurValue += nValue;
    if ( nCurValue > nMaxValue )
    {
        nCurValue = nMaxValue;
    }
    m_pKernelModule->SetPropertyInt( self, "VP", nCurValue );

    return nCurValue;
}

int NFCPropertyModule::ConsumeActivityVP( const NFIDENTID& self, int nValue )
{
    int nVP = m_pKernelModule->QueryPropertyInt( self, "ActivityVP" );
    if ( ( nVP > 0 ) && ( nVP - nValue >= 0 ) )
    {
        nVP -= nValue;
        m_pKernelModule->SetPropertyInt( self, "ActivityVP", nVP );

        return nValue;
    }

    return -1;
}

int NFCPropertyModule::FullActivityVP( const NFIDENTID& self )
{
    int nMaxVP = m_pKernelModule->QueryPropertyInt( self, "MAXActivityVP" );
    if ( nMaxVP > 0 )
    {
        m_pKernelModule->SetPropertyInt( self, "ActivityVP", nMaxVP );

        return 0;
    }

    return 0;
}

int NFCPropertyModule::AddActivityVP( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "ActivityVP" );
    int nMaxValue = m_pKernelModule->QueryPropertyInt( self, "MAXActivityVP" );

    nCurValue += nValue;
    if ( nCurValue > nMaxValue )
    {
        nCurValue = nMaxValue;
    }
    m_pKernelModule->SetPropertyInt( self, "ActivityVP", nCurValue );

    return nCurValue;
}

int NFCPropertyModule::OnHPRecoverHeart( const NFIDENTID& self, const NFIValueList& var )
{
    return 0;
}

int NFCPropertyModule::OnMPRecoverHeart( const NFIDENTID& self, const NFIValueList& var )
{
    return 0;
}

int NFCPropertyModule::OnSPRecoverHeart( const NFIDENTID& self, const NFIValueList& var )
{
    return 0;
}

int NFCPropertyModule::OnVPRecoverHeart( const NFIDENTID& self, const NFIValueList& var )
{
    int nValue = m_pKernelModule->QueryPropertyInt(self, "VPREGEN");
    AddVP(self, nValue);

    return 0;
}

int NFCPropertyModule::OnActivityVPRecoverHeart( const NFIDENTID& self, const NFIValueList& var )
{
    int nValue = m_pKernelModule->QueryPropertyInt(self, "ActivityVP");
    AddActivityVP(self, nValue);

    return 0;
}

int NFCPropertyModule::ConsumeMoney( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "Money" );
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt( self, "Money", nCurValue);

        return nValue;
    }    

    return 0;
}

int NFCPropertyModule::AddMoney( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "Money" );
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt( self, "Money", nCurValue);

    return nCurValue;
}

int NFCPropertyModule::ConsumeSoulStone( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "SoulStone" );
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt( self, "SoulStone", nCurValue);

        return nValue;
    }    

    return -1;
}

int NFCPropertyModule::AddSoulStone( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "SoulStone" );
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt( self, "SoulStone", nCurValue);

    return nCurValue;
}

int NFCPropertyModule::ConsumeEssence( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "EssenceValue" );
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        m_pKernelModule->SetPropertyInt( self, "EssenceValue", nCurValue);

        return nValue;
    }    

    return -1;
}

int NFCPropertyModule::AddEssence( const NFIDENTID& self, int nValue )
{
    int nCurValue = m_pKernelModule->QueryPropertyInt( self, "EssenceValue" );
    nCurValue += nValue;
    m_pKernelModule->SetPropertyInt( self, "EssenceValue", nCurValue);

    return nCurValue;
}

int NFCPropertyModule::OnReqModifyData( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
{
    if (var.GetCount() != 6
        || !var.TypeEx(VARIANT_TYPE::VTYPE_OBJECT, VARIANT_TYPE::VTYPE_OBJECT, VARIANT_TYPE::VTYPE_STRING, 
        VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_INT, VARIANT_TYPE::VTYPE_STRING, VARIANT_TYPE::VTYPE_UNKNOWN))
    {
        return 1;
    }

    NFIDENTID identSelf = var.ObjectVal(0);
    NFIDENTID identOther = var.ObjectVal(1);
    const std::string& strName = var.StringVal(2);
    int nType = var.IntVal(3);
    int nValue = var.IntVal(4);
    const std::string& strKey = var.StringVal(5);

    if (identSelf == self
        && identSelf == identOther)
    {

		if(strName == "YBP" || strName == "Cash")
		{
			NFIDENTID nCurValue  = m_pKernelModule->QueryPropertyObject(self, strName);
			if (1 == nType)
			{
				m_pKernelModule->SetPropertyObject( self, strName, nValue + nCurValue.nData64);
			}
			else if (2 == nType)
			{
				m_pKernelModule->SetPropertyObject( self, strName, nCurValue.nData64 - nValue );
			}
		}

        int nCurValue = m_pKernelModule->QueryPropertyInt( self, strName );
        if (0 == nType)
        {
            m_pKernelModule->SetPropertyInt( self, strName, nValue);
        }
        else if (1 == nType)
        {
            m_pKernelModule->SetPropertyInt( self, strName, nValue + nCurValue);
        }
        else if (2 == nType)
        {
            m_pKernelModule->SetPropertyInt( self, strName, nCurValue - nValue );
        }
    }

    return 0;
}

int NFCPropertyModule::AddHonour( const NFIDENTID& self, int nValue )
{
	if (nValue <= 0)
	{
		return -1;
	}

	int nCurValue = m_pKernelModule->QueryPropertyInt( self, "Honour" );
	nCurValue += nValue;
	m_pKernelModule->SetPropertyInt( self, "Honour", nCurValue);

	return nCurValue;
}

int NFCPropertyModule::ConsumeHonour( const NFIDENTID& self, int nValue )
{

	if (nValue <= 0)
	{
		return -1;
	}
	
	int nCurValue = m_pKernelModule->QueryPropertyInt( self, "Honour" );
	nCurValue -= nValue;

	if (nCurValue < 0)
	{
		return -1;
	}
	
	m_pKernelModule->SetPropertyInt( self, "Honour", nCurValue);

	return nCurValue;

}

int NFCPropertyModule::ConsumeYBP( const NFIDENTID& self, int nValue )
{
    NFIDENTID ident = m_pKernelModule->QueryPropertyObject( self, "YBP" );
    int nCurValue = ident.nData64;
    nCurValue -= nValue;
    if (nCurValue >= 0)
    {
        ident.nData64 = nCurValue;
        m_pKernelModule->SetPropertyObject( self, "YBP", ident);

        return nValue;
    }    

    return 0;
}
