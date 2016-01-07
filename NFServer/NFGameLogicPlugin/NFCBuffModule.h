// -------------------------------------------------------------------------
//    @FileName      :    NFCBuffModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCBuffModule
//
// -------------------------------------------------------------------------

#ifndef NFC_BUFF_MODULE_H
#define NFC_BUFF_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIGameLogicModule.h"
#include "NFComm/NFPluginModule/NFIEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIBuffModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

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
    virtual bool Execute();
    virtual bool AfterInit();


    //添加
    virtual bool AddBuff( const NFGUID& self, const std::string& strConfig, const NFGUID& releaserIdent );

    //删除
    virtual bool RemoveBuff( const NFGUID& self, const std::string& strConfig );

    //删除某类BUFF
    virtual bool RemoveBuff( const NFGUID& self, NFIBuffConfigModule::BuffType eType );

    //瞬间删除所有的控制类BUFF
    virtual bool SelfFree( const NFGUID& self );

    //删除影响某个属性的某类BUFF
    //virtual bool RemoveBuff(BuffType buffType, BuffSubType subType, const std::string& strProperty);
    //是否有这个BUFF
    virtual bool HasBuff( const NFGUID& self, const std::string& strConfig );

    //更新
    virtual bool Execute( const NFGUID& self, float fPassTime );

    //攻击时
    virtual int Attack( const NFGUID& self, const NFGUID& other );

    //死亡时
    virtual int OnDead( const NFGUID& self );

protected:
    //应用BUFF效果
    virtual bool ApplyRoleEffectValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //处理一个buff(当添加时)
    virtual int ProcessBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //处理buff的属性值
    virtual int ProcessBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //处理buff的属性值--绝对值
    virtual int ProcessBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //处理buff的属性值--百分比值
    virtual int ProcessBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //处理buff的控制值
    virtual int ProcessBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //处理buff的控制值--绝对值[控制值无百分比处理]
    virtual int ProcessBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    ///////////////////////////////////////////////////////////////////////////

    //还原buff
    virtual int ReverseBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //还原buff属性值
    virtual int ReverseBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //////////////////////////////////////////////////////////////////////////

    //处理一个debuff(当添加时)
    virtual int ProcessDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //处理buff的属性值
    virtual int ProcessDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //处理buff的属性值--绝对值
    virtual int ProcessDeBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //处理buff的属性值--百分比
    virtual int ProcessDeBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //处理debuff的控制值-------理论上说不允许配这个
    virtual int ProcessDeBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //处理debuff的控制值--绝对值[控制值无百分比处理]-------理论上说不允许配这个
    virtual int ProcessDeBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //还原debuff
    virtual int ReverseDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //还原debufff属性值
    virtual int ReverseDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );

    //还原控制值
    virtual int ReverseControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //还原控制值--绝对值
    virtual int ReverseControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );

    //得到那个组的当前的buffconfig
    virtual std::string GetGroupBuffIndex( const NFGUID& self, int nGroupID );

    //设置时间
    virtual void SetBuffTime( const NFGUID& self, const std::string& strConfig, int nRemainTime, float fIntervalTime );

private:

    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIBuffConfigModule* m_pBuffConfigModule;

    char* mstrRunTimeEffectTable;
    char* mstrPropertyTable;
};


#endif
