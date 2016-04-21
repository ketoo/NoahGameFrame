// -------------------------------------------------------------------------
//    @FileName         ：    NFIScriptModule.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-07-08
//    @Module           ：    NFIScriptModule
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
	virtual int DoHeartBeatCommonCB(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount)
	{
		NFCSriptData* pScriptData = GetElement(self);
		if (pScriptData)
		{
			NFList<NFCScriptName>* pList = pScriptData->mxHeartBeatData.GetElement(strHeartBeat);
			if (pList)
			{
				NFCScriptName xScriptName;
				bool bRet = pList->First(xScriptName);
				while (bRet)
				{
					DoHeartBeatScript(self, strHeartBeat, fTime, nCount, xScriptName.strComponentName, xScriptName.strFunctionName);

					bRet = pList->Next(xScriptName);
				}
			}
		}
		return 0;
	}

	virtual int DoEventCommonCB(const NFGUID& self, const int nEventID, const NFIDataList& var)
	{
		NFCSriptData* pScriptData = GetElement(self);
		if (pScriptData)
		{
			NFList<NFCScriptName>* pList = pScriptData->mxEventData.GetElement(nEventID);
			if (pList)
			{
				NFCScriptName xScriptName;
				bool bRet = pList->First(xScriptName);
				while (bRet)
				{
					DoEventScript(self, nEventID, xScriptName.strComponentName, xScriptName.strFunctionName, var);

					bRet = pList->Next(xScriptName);
				}
			}
		}

		return 0;
	}

	virtual int DoPropertyCommEvent(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
	{
		NFCSriptData* pScriptData = GetElement(self);
		if (pScriptData)
		{
			NFList<NFCScriptName>* pList = pScriptData->mxPropertyCallBackData.GetElement(strPropertyName);
			if (pList)
			{
				NFCScriptName xScriptName;
				bool bRet = pList->First(xScriptName);
				while (bRet)
				{
					DoScriptPropertyCallBack(self, strPropertyName, xScriptName.strComponentName, xScriptName.strFunctionName, oldVar, newVar);

					bRet = pList->Next(xScriptName);
				}
			}
		}

		return 0;
	}

	virtual int DoRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
	{
		NFCSriptData* pScriptData = GetElement(self);
		if (pScriptData)
		{
			NFList<NFCScriptName>* pList = pScriptData->mxRecordCallBackData.GetElement(xEventData.strRecordName);
			if (pList)
			{
				NFCScriptName xScriptName;
				bool bRet = pList->First(xScriptName);
				while (bRet)
				{
					DoScriptRecordCallBack(self, xEventData.strRecordName, xScriptName.strComponentName, xScriptName.strFunctionName, xEventData.nOpType, xEventData.nRow, xEventData.nCol, oldVar, newVar);

					bRet = pList->Next(xScriptName);
				}
			}
		}

		return 0;
	}

	virtual int DoClassCommonEvent(NFILogicClassModule* pLogicClassModule, const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
	{
		NF_SHARE_PTR<NFIComponentManager> pComponentManager = pLogicClassModule->GetClassComponentManager(strClassName);
		if (pComponentManager.get())
		{
			NF_SHARE_PTR<NFIComponent> pComponent = pComponentManager->First();
			while (pComponent.get() && pComponent->Enable())
			{
				DoClassCommonScript(self, pComponent->GetComponentName(), strClassName, eClassEvent);

				pComponent = pComponentManager->Next();
			}
		}

		return 0;
	}

	//call script
	virtual int DoScript(const NFGUID& self, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg) = 0;
	virtual int DoClassCommonScript(const NFGUID& self, const std::string& strComponentName, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent) = 0;
	virtual int DoEventScript(const NFGUID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction, const NFCDataList& arg) = 0;
	virtual int DoHeartBeatScript(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount, std::string& strComponentName, const std::string& strFunction) = 0;

	virtual int DoScriptPropertyCallBack(const NFGUID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction, const NFCDataList::TData& oldVar, const NFCDataList::TData& newVar) = 0;
	virtual int DoScriptRecordCallBack(const NFGUID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction, const int nOpType, const int nRow, const int nCol, const NFCDataList::TData& oldVar, const NFCDataList::TData& newVar) = 0;

	// operating function
	virtual bool AddPropertyCallBack(const NFGUID& self, const std::string& strPropertyName, const std::string& strComponentName, const std::string& strFunction)
	{
		NFCSriptData* pScriptData = GetElement(self);
		if (!pScriptData)
		{
			pScriptData = new NFCSriptData();
			AddElement(self, pScriptData);
		}

		NFList<NFCScriptName>* pScriptNameList = pScriptData->mxPropertyCallBackData.GetElement(strPropertyName);
		if (!pScriptNameList)
		{
			pScriptNameList = new NFList<NFCScriptName>();
			pScriptData->mxPropertyCallBackData.AddElement(strPropertyName, pScriptNameList);
		}

		NFCScriptName xScriptName(strComponentName, strFunction);
		pScriptNameList->Add(xScriptName);

		NFIKernelModule* pKernelModule = GetPluginManager()->FindModule<NFIKernelModule>("NFCKernelModule");
		pKernelModule->AddPropertyCallBack(self, strPropertyName, this, &NFIScriptModule::OnPropertyCB);

		return true;
	}

	int OnPropertyCB(const NFGUID& self, const std::string& strPropertyName, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
	{
		return DoPropertyCommEvent(self, strPropertyName, oldVar, newVar);
	}

	virtual bool AddRecordCallBack(const NFGUID& self, const std::string& strRecordName, const std::string& strComponentName, const std::string& strFunction)
	{
		NFCSriptData* pScriptData = GetElement(self);
		if (!pScriptData)
		{
			pScriptData = new NFCSriptData();
			AddElement(self, pScriptData);
		}

		NFList<NFCScriptName>* pScriptNameList = pScriptData->mxRecordCallBackData.GetElement(strRecordName);
		if (!pScriptNameList)
		{
			pScriptNameList = new NFList<NFCScriptName>();
			pScriptData->mxRecordCallBackData.AddElement(strRecordName, pScriptNameList);
		}

		NFCScriptName xScriptName(strComponentName, strFunction);
		pScriptNameList->Add(xScriptName);

		NFIKernelModule* pKernelModule = GetPluginManager()->FindModule<NFIKernelModule>("NFCKernelModule");
		pKernelModule->AddRecordCallBack(self, strRecordName, this, &NFIScriptModule::OnRecordCB);

		return true;
	}

	int OnRecordCB(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
	{
		return DoRecordCommonEvent(self, xEventData, oldVar, newVar);
	}

	virtual bool AddHeartBeatCallBack(const NFGUID& self, const std::string& strHeartBeatName, const std::string& strComponentName, const std::string& strFunction, const float fTime, const int nCount)
	{
		NFCSriptData* pScriptData = GetElement(self);
		if (!pScriptData)
		{
			pScriptData = new NFCSriptData();
			AddElement(self, pScriptData);
		}

		NFList<NFCScriptName>* pScriptNameList = pScriptData->mxHeartBeatData.GetElement(strHeartBeatName);
		if (!pScriptNameList)
		{
			pScriptNameList = new NFList<NFCScriptName>();
			pScriptData->mxHeartBeatData.AddElement(strHeartBeatName, pScriptNameList);
		}

		//应该是同时直接向系统注册.
		NFCScriptName xScriptName(strComponentName, strFunction);
		pScriptNameList->Add(xScriptName);

		NFIKernelModule* pKernelModule = GetPluginManager()->FindModule<NFIKernelModule>("NFCKernelModule");
		pKernelModule->AddHeartBeat(self, strHeartBeatName, this, &NFIScriptModule::OnHeartBeatCB, fTime, nCount);

		return true;
	}

	int OnHeartBeatCB(const NFGUID& self, const std::string& strHeartBeatName, const float fTime, const int nCount)
	{
		return DoHeartBeatCommonCB(self, strHeartBeatName, fTime, nCount);
	}

	virtual bool AddEventCallBack(const NFGUID& self, const int nEventID, const std::string& strComponentName, const std::string& strFunction)
	{
		NFCSriptData* pScriptData = GetElement(self);
		if (!pScriptData)
		{
			pScriptData = new NFCSriptData();
			AddElement(self, pScriptData);
		}

		NFList<NFCScriptName>* pScriptNameList = pScriptData->mxEventData.GetElement(nEventID);
		if (!pScriptNameList)
		{
			pScriptNameList = new NFList<NFCScriptName>();
			pScriptData->mxEventData.AddElement(nEventID, pScriptNameList);
		}

		NFCScriptName xScriptName(strComponentName, strFunction);
		pScriptNameList->Add(xScriptName);

		NFIKernelModule* pKernelModule = GetPluginManager()->FindModule<NFIKernelModule>("NFCKernelModule");
		pKernelModule->AddEventCallBack(self, nEventID, this, &NFIScriptModule::OnEventCB);

		return true;
	}

	int OnEventCB(const NFGUID& self, const int nEventID, const NFIDataList& argVar)
	{
		return DoEventCommonCB(self, nEventID, argVar);
	}
};

#endif
