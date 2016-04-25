// -------------------------------------------------------------------------
//    @FileName         £º    NFIScriptModule.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-07-08
//    @Module           £º    NFIScriptModule
//    @Desc             :     every script plugin need to export two interfaces(NFCScriptVarList and most of NFCScriptKernel)
// -------------------------------------------------------------------------

#ifndef NFI_SCRIPT_MODULE_H
#define NFI_SCRIPT_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIElementInfoModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"
#include "NFComm/NFPluginModule/NFILuaScriptModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFCScriptName
{
public:
	bool operator < (const NFCScriptName& id) const
	{
		return this->strComponentName.length() < id.strComponentName.length() && this->strFunctionName.length() < id.strFunctionName.length();
	}

	bool operator == (const NFCScriptName& id) const
	{
		if (this->strComponentName != id.strComponentName
			|| this->strFunctionName != id.strFunctionName)
		{
			return false;
		}

		return true;
	}

	NFCScriptName() {}

	NFCScriptName(const std::string& strComponentName, const std::string& strFunctionName)
	{
		this->strComponentName = strComponentName;
		this->strFunctionName = strFunctionName;
	}

	std::string strComponentName;
	std::string strFunctionName;
};


class NFCSriptData
{
public:
	//eventlist: id->ScriptName,FunctionName
	NFMap<int, NFList<NFCScriptName>> mxEventData;

	//heartbeatlist: name->ScriptName,FunctionName
	NFMap<std::string, NFList<NFCScriptName>> mxHeartBeatData;

	//propertycblist: name->ScriptName,FunctionName
	NFMap<std::string, NFList<NFCScriptName>> mxPropertyCallBackData;

	//recordcblist: name->ScriptName,FunctionName
	NFMap<std::string, NFList<NFCScriptName>> mxRecordCallBackData;

protected:
private:
};

class NFIScriptModule
	: public NFILogicModule, public NFMap<NFGUID, NFCSriptData>
{

public:

};

#endif
