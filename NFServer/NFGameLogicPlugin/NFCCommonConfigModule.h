// -------------------------------------------------------------------------
//    @FileName      :    NFCCommonConfigModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-07-05
//    @Module           :    NFCCommonConfigModule
//
// -------------------------------------------------------------------------

#ifndef NFC_COMMONCONFIG_MODULE_H
#define NFC_COMMONCONFIG_MODULE_H

#include "NFComm/NFPluginModule/NFICommonConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCCommonConfigModule
    : public NFICommonConfigModule
{
public:
public:
	class CAttributeList : public NFMapEx<std::string, std::string>
	{
	public:
		const int GetInt(const std::string& strName)
		{
			NF_SHARE_PTR<std::string> pStr = GetElement(strName);
			if (pStr && !pStr->empty())
			{
				int nData = 0;
				NF_StrTo(*pStr, nData);
				return nData;
			}

			return 0;
		}

		const std::string& GetString(const std::string& strName)
		{
			NF_SHARE_PTR<std::string> pStr = GetElement(strName);
			if (pStr)
			{
				return *pStr;
			}

			return NULL_STR;
		}
	};

	struct CStructInfo : public NFMapEx<std::string, CAttributeList>
	{
		CAttributeList  mmStructAttribute;
	};

public:
    NFCCommonConfigModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCCommonConfigModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	virtual bool ClearConfig();
	virtual const int GetAttributeInt(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute);
	virtual const int GetAttributeInt(const std::string& strStructName, const std::string& strSDKAttribute);
	virtual const std::string& GetAttributeString(const std::string& strStructName, const std::string& strStructItemName, const std::string& strAttribute);
	virtual const std::string& GetAttributeString(const std::string& strStructName,const std::string& strSDKAttribute);
	virtual bool LoadConfig(const std::string& strFile);
	virtual const int GetStructItemList(const std::string&strStructName, std::vector<std::string>& xList);

private:
	NFMapEx<std::string, CStructInfo> mmData; //strStructName<-->CStructInfo
};


#endif
