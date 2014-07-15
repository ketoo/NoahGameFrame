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


    //���
    virtual bool AddBuff( const NFIDENTID& self, const std::string& strConfig, const NFIDENTID& releaserIdent );

    //ɾ��
    virtual bool RemoveBuff( const NFIDENTID& self, const std::string& strConfig );

    //ɾ��ĳ��BUFF
    virtual bool RemoveBuff( const NFIDENTID& self, NFIBuffConfigModule::BuffType eType );

    //˲��ɾ�����еĿ�����BUFF
    virtual bool SelfFree( const NFIDENTID& self );

    //ɾ��Ӱ��ĳ�����Ե�ĳ��BUFF
    //virtual bool RemoveBuff(BuffType buffType, BuffSubType subType, const std::string& strProperty);
    //�Ƿ������BUFF
    virtual bool HasBuff( const NFIDENTID& self, const std::string& strConfig );

    //����
    virtual bool Execute( const NFIDENTID& self, float fPassTime );

    //����ʱ
    virtual int Attack( const NFIDENTID& self, const NFIDENTID& other );

    //����ʱ
    virtual int OnDead( const NFIDENTID& self );

protected:
    //Ӧ��BUFFЧ��
    virtual bool ApplyRoleEffectValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //����һ��buff(�����ʱ)
    virtual int ProcessBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //����buff������ֵ
    virtual int ProcessBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //����buff������ֵ--����ֵ
    virtual int ProcessBuffValuePropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //����buff������ֵ--�ٷֱ�ֵ
    virtual int ProcessBuffValuePropertyReferRatioValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //����buff�Ŀ���ֵ
    virtual int ProcessBuffControlProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //����buff�Ŀ���ֵ--����ֵ[����ֵ�ްٷֱȴ���]
    virtual int ProcessBuffControlPropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    ///////////////////////////////////////////////////////////////////////////

    //��ԭbuff
    virtual int ReverseBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //��ԭbuff����ֵ
    virtual int ReverseBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //////////////////////////////////////////////////////////////////////////

    //����һ��debuff(�����ʱ)
    virtual int ProcessDeBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //����buff������ֵ
    virtual int ProcessDeBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //����buff������ֵ--����ֵ
    virtual int ProcessDeBuffValuePropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //����buff������ֵ--�ٷֱ�
    virtual int ProcessDeBuffValuePropertyReferRatioValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //����debuff�Ŀ���ֵ-------������˵�����������
    virtual int ProcessDeBuffControlProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );
    //����debuff�Ŀ���ֵ--����ֵ[����ֵ�ްٷֱȴ���]-------������˵�����������
    virtual int ProcessDeBuffControlPropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig, const NFIDENTID& releaserIdent );

    //��ԭdebuff
    virtual int ReverseDeBuffValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //��ԭdebufff����ֵ
    virtual int ReverseDeBuffValueProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );

    //��ԭ����ֵ
    virtual int ReverseControlProperty( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );
    //��ԭ����ֵ--����ֵ
    virtual int ReverseControlPropertyReferAbsoluteValue( const NFIDENTID& self, NFIBuffConfigModule::NFCBuffConfig* pBuffConfig );

    //�õ��Ǹ���ĵ�ǰ��buffconfig
    virtual std::string GetGroupBuffIndex( const NFIDENTID& self, int nGroupID );

    //����ʱ��
    virtual void SetBuffTime( const NFIDENTID& self, const std::string& strConfig, int nRemainTime, float fIntervalTime );

private:

    NFIEventProcessModule* m_pEventProcessModule;
    NFIKernelModule* m_pKernelModule;
    NFIBuffConfigModule* m_pBuffConfigModule;

    char* mstrRunTimeEffectTable;
    char* mstrPropertyTable;
};


#endif
