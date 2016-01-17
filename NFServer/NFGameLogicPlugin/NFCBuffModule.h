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
#include "NFComm/NFPluginModule/NFIBuffModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

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


    //���
    virtual bool AddBuff( const NFGUID& self, const std::string& strConfig, const NFGUID& releaserIdent );

    //ɾ��
    virtual bool RemoveBuff( const NFGUID& self, const std::string& strConfig );

    //ɾ��ĳ��BUFF
    virtual bool RemoveBuff( const NFGUID& self, NFIBuffConfigModule::BuffType eType );

    //˲��ɾ�����еĿ�����BUFF
    virtual bool SelfFree( const NFGUID& self );

    //ɾ��Ӱ��ĳ�����Ե�ĳ��BUFF
    //virtual bool RemoveBuff(BuffType buffType, BuffSubType subType, const std::string& strProperty);
    //�Ƿ������BUFF
    virtual bool HasBuff( const NFGUID& self, const std::string& strConfig );

    //����
    virtual bool Execute( const NFGUID& self, float fPassTime );

    //����ʱ
    virtual int Attack( const NFGUID& self, const NFGUID& other );

    //����ʱ
    virtual int OnDead( const NFGUID& self );

protected:
    //Ӧ��BUFFЧ��
    virtual bool ApplyRoleEffectValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //����һ��buff(�����ʱ)
    virtual int ProcessBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //����buff������ֵ
    virtual int ProcessBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //����buff������ֵ--����ֵ
    virtual int ProcessBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //����buff������ֵ--�ٷֱ�ֵ
    virtual int ProcessBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //����buff�Ŀ���ֵ
    virtual int ProcessBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //����buff�Ŀ���ֵ--����ֵ[����ֵ�ްٷֱȴ���]
    virtual int ProcessBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    ///////////////////////////////////////////////////////////////////////////

    //��ԭbuff
    virtual int ReverseBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //��ԭbuff����ֵ
    virtual int ReverseBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //////////////////////////////////////////////////////////////////////////

    //����һ��debuff(�����ʱ)
    virtual int ProcessDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //����buff������ֵ
    virtual int ProcessDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //����buff������ֵ--����ֵ
    virtual int ProcessDeBuffValuePropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //����buff������ֵ--�ٷֱ�
    virtual int ProcessDeBuffValuePropertyReferRatioValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //����debuff�Ŀ���ֵ-------������˵�����������
    virtual int ProcessDeBuffControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );
    //����debuff�Ŀ���ֵ--����ֵ[����ֵ�ްٷֱȴ���]-------������˵�����������
    virtual int ProcessDeBuffControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFGUID& releaserIdent );

    //��ԭdebuff
    virtual int ReverseDeBuffValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //��ԭdebufff����ֵ
    virtual int ReverseDeBuffValueProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );

    //��ԭ����ֵ
    virtual int ReverseControlProperty( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //��ԭ����ֵ--����ֵ
    virtual int ReverseControlPropertyReferAbsoluteValue( const NFGUID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );

    //�õ��Ǹ���ĵ�ǰ��buffconfig
    virtual std::string GetGroupBuffIndex( const NFGUID& self, int nGroupID );

    //����ʱ��
    virtual void SetBuffTime( const NFGUID& self, const std::string& strConfig, int nRemainTime, float fIntervalTime );

private:

    NFIKernelModule* m_pKernelModule;
    NFIBuffConfigModule* m_pBuffConfigModule;

    char* mstrRunTimeEffectTable;
};


#endif
