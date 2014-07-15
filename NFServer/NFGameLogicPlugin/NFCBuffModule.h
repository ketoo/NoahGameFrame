// -------------------------------------------------------------------------
//    @FileName      :    NFCBuffModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCBuffModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_BUFF_MODULE_H_
#define _NFC_BUFF_MODULE_H_

#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIBuffModule.h"

class NFCBuffModule
    : public NFIBuffModule
{
public:
    enum RUNTIME_BUFF_INFO
    {
        RTBI_BUFFCONFIG,//string
        RTBI_RELEASER,//object
        RTBI_INTERVALTIME,//float
        RTBI_REMAINTIME,//int
        RTBI_EFFECTVALUE,//string
    };

    NFCBuffModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCBuffModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();


    //添加
    virtual bool AddBuff( const NFIDENTID& self, const std::string& strConfig, const NFIDENTID& releaserIdent );

    //删除
    virtual bool RemoveBuff( const NFIDENTID& self, const std::string& strConfig );

    //删除某类BUFF
    virtual bool RemoveBuff( const NFIDENTID& self, NFIBuffConfigModule::BuffType eType );

    //瞬间删除所有的控制类BUFF
    virtual bool SelfFree( const NFIDENTID& self );

    //删除影响某个属性的某类BUFF
    //virtual bool RemoveBuff(BuffType buffType, BuffSubType subType, const std::string& strProperty);
    //是否有这个BUFF
    virtual bool HasBuff( const NFIDENTID& self, const std::string& strConfig );

    //更新
    virtual bool Execute( const NFIDENTID& self, float fPassTime );

    //攻击时
    virtual int Attack( const NFIDENTID& self, const NFIDENTID& other );

    //死亡时
    virtual int OnDead( const NFIDENTID& self );

protected:
    //应用BUFF效果
    virtual bool ApplyRoleEffectValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //处理一个buff(当添加时)
    virtual int ProcessBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //处理buff的属性值
    virtual int ProcessBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //处理buff的属性值--绝对值
    virtual int ProcessBuffValuePropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //处理buff的属性值--百分比值
    virtual int ProcessBuffValuePropertyReferRatioValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //处理buff的控制值
    virtual int ProcessBuffControlProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //处理buff的控制值--绝对值[控制值无百分比处理]
    virtual int ProcessBuffControlPropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    ///////////////////////////////////////////////////////////////////////////

    //还原buff
    virtual int ReverseBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //还原buff属性值
    virtual int ReverseBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //////////////////////////////////////////////////////////////////////////

    //处理一个debuff(当添加时)
    virtual int ProcessDeBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //处理buff的属性值
    virtual int ProcessDeBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //处理buff的属性值--绝对值
    virtual int ProcessDeBuffValuePropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //处理buff的属性值--百分比
    virtual int ProcessDeBuffValuePropertyReferRatioValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //处理debuff的控制值-------理论上说不允许配这个
    virtual int ProcessDeBuffControlProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //处理debuff的控制值--绝对值[控制值无百分比处理]-------理论上说不允许配这个
    virtual int ProcessDeBuffControlPropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //还原debuff
    virtual int ReverseDeBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //还原debufff属性值
    virtual int ReverseDeBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );

    //还原控制值
    virtual int ReverseControlProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //还原控制值--绝对值
    virtual int ReverseControlPropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );

    //得到那个组的当前的buffconfig
    virtual std::string GetGroupBuffIndex( const NFIDENTID& self, int nGroupID );

    //设置时间
    virtual void SetBuffTime( const NFIDENTID& self, const std::string& strConfig, int nRemainTime, float fIntervalTime );

private:

    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIBuffConfigModule* m_pBuffConfigModule;

    char* mstrRunTimeEffectTable;
    char* mstrPropertyTable;
};


#endif
