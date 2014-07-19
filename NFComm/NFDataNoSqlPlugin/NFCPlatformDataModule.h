// -------------------------------------------------------------------------
//    @FileName      :    NFCPlatformDataModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-11
//    @Module           :    NFCPlatformDataModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_PLATFORM_DATA_MODULE_H_
#define _NFC_PLATFORM_DATA_MODULE_H_

#include "NFCDataNoSqlDriver.h"
#include "NFComm/NFCore/NFPlatform.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIPlatformDataModule.h"

class NFCPlatformDataModule
    : public NFIPlatformDataModule
{
public:

    NFCPlatformDataModule(NFIPluginManager* p);
    virtual ~NFCPlatformDataModule();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLasFrametime, const float fStartedTime);
    virtual bool AfterInit();

    virtual bool ConnectSql(const std::string& strIP);

	virtual int AddAccountInfo(const std::string& strAccount, const std::string& strPassword);

	//int& nState:0Ϊ��ȷ�ɵ�¼��>0Ϊ�������ִ���,�����ŵ�
	virtual int ConfirmAccountInfo(const std::string& strAccount, const std::string& strPassword);

	virtual int SetAccountState(const std::string& strAccount, const int nState);

    virtual NFIDataNoSqlDriver* GetDriver()
    {
        return m_pPlayerDataDriver;
    }

protected:
	NFIDataNoSqlDriver* m_pPlayerDataDriver;

};

#endif