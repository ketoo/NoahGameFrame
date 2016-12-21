// -------------------------------------------------------------------------
//    @FileName			:    NFCSceneAOIModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-05
//    @Module           :    NFCSceneAOIModule
//
// -------------------------------------------------------------------------

#include "NFCSceneAOIModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCSceneAOIModule::Init()
{
    return true;
}

bool NFCSceneAOIModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();

	m_pKernelModule->RegisterCommonClassEvent(this, &NFCSceneAOIModule::OnClassCommonEvent);
	m_pKernelModule->RegisterCommonPropertyEvent(this, &NFCSceneAOIModule::OnPropertyCommonEvent);
	m_pKernelModule->RegisterCommonRecordEvent(this, &NFCSceneAOIModule::OnRecordCommonEvent);

	//init all scene
	NF_SHARE_PTR<NFIClass> pLogicClass = m_pClassModule->GetElement(NFrame::Scene::ThisName());
	if (pLogicClass)
	{
		NFList<std::string>& strIdList = pLogicClass->GetIdList();

		std::string strId;
		bool bRet = strIdList.First(strId);
		while (bRet)
		{
			int nSceneID = lexical_cast<int>(strId);
			m_pKernelModule->CreateScene(nSceneID);

			bRet = strIdList.Next(strId);
		}
	}
    return true;
}

bool NFCSceneAOIModule::BeforeShut()
{
    return true;
}

bool NFCSceneAOIModule::Shut()
{
    return true;
}

bool NFCSceneAOIModule::Execute()
{
    return true;
}

bool NFCSceneAOIModule::RequestEnterScene(const NFGUID & self, const int nSceneID, const int nType, const NFIDataList & argList)
{
	return RequestEnterScene(self, nSceneID, -1, nType, argList);
}

bool NFCSceneAOIModule::RequestEnterScene(const NFGUID & self, const int nSceneID, const int nGrupID, const int nType, const NFIDataList & argList)
{
	const int nNowSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
	const int nNowGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());
	
	if (nNowSceneID == nSceneID
		&& nNowGroupID == nGrupID)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "in same scene and group but it not a clone scene", nSceneID);

		return false;
	}

	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = GetElement(nSceneID);
	if (!pSceneInfo)
	{
		return false;
	}

	NFINT64 nNewGroupID = nGrupID;
	if (nGrupID < 0)
	{
		//call in inner environments
		nNewGroupID = m_pKernelModule->RequestGroupScene(nSceneID);
	}
	
	int nEnterConditionCode = BeforeEnterScene(self, nSceneID, nNewGroupID, nType, argList);
	if (nEnterConditionCode != 0)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "before enter condition code:", nEnterConditionCode);
		return false;
	}

	////////////////////////////////////

	NF_SHARE_PTR<SceneSeedResource> pResource = pSceneInfo->mtSceneResourceConfig.First();
	while (pResource)
	{
		const std::string& strClassName = m_pElementModule->GetPropertyString(pResource->strConfigID, NFrame::NPC::ClassName());

		NFCDataList arg;
		arg << NFrame::NPC::X() << pResource->vSeedPos.X();
		arg << NFrame::NPC::Y() << pResource->vSeedPos.Y();
		arg << NFrame::NPC::Z() << pResource->vSeedPos.Z();
		arg << NFrame::NPC::SeedID() << pResource->strSeedID;

		m_pKernelModule->CreateObject(NFGUID(), nSceneID, nNewGroupID, strClassName, pResource->strConfigID, arg);

		pResource = pSceneInfo->mtSceneResourceConfig.Next();
	}

	///////////////////////////////

	if (!m_pKernelModule->SwitchScene(self, nSceneID, nNewGroupID, 0.0f, 0.0f, 0.0f, 0.0f, argList))
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "SwitchScene failed", nSceneID);

		return false;
	}

	int nAfterConditionCode = AfterEnterScene(self, nSceneID, nNewGroupID, nType, argList);
	if (nAfterConditionCode != 0)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "After enter scene condition code:", nAfterConditionCode);
	}

	return true;
}

bool NFCSceneAOIModule::AddSeedData(const int nSceneID, const std::string & strSeedID, const std::string & strConfigID, const NFVector3 & vPos)
{
	NF_SHARE_PTR<NFCSceneInfo> pSceneInfo = GetElement(nSceneID);
	if (pSceneInfo)
	{
		return pSceneInfo->AddSeedObjectInfo(strSeedID, strConfigID, vPos);
	}

	return false;
}

bool NFCSceneAOIModule::AddObjectEnterCallBack(const OBJECT_ENTER_EVENT_FUNCTOR_PTR & cb)
{
	mtObjectEnterCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddObjectLeaveCallBack(const OBJECT_LEAVE_EVENT_FUNCTOR_PTR & cb)
{
	mtObjectLeaveCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddPropertyEnterCallBack(const PROPERTY_ENTER_EVENT_FUNCTOR_PTR & cb)
{
	mtPropertyEnterCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddRecordEnterCallBack(const RECORD_ENTER_EVENT_FUNCTOR_PTR & cb)
{
	mtRecordEnterCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddPropertyEventCallBack(const PROPERTY_SINGLE_EVENT_FUNCTOR_PTR & cb)
{
	mtPropertySingleCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddRecordEventCallBack(const RECORD_SINGLE_EVENT_FUNCTOR_PTR & cb)
{
	mtRecordSingleCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddBeforeEnterSceneCallBack(const BEFORE_ENTER_SCENE_FUNCTOR_PTR & cb)
{
	mtBeforeEnterSceneCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddAfterEnterSceneCallBack(const AFTER_ENTER_SCENE_FUNCTOR_PTR & cb)
{
	mtAfterEnterSceneCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddBeforeLeaveSceneCallBack(const BEFORE_LEAVE_SCENE_FUNCTOR_PTR & cb)
{
	mtBeforeLeaveSceneCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::AddAfterLeaveSceneCallBack(const AFTER_LEAVE_SCENE_FUNCTOR_PTR & cb)
{
	mtAfterLeaveSceneCallback.push_back(cb);
	return true;
}

bool NFCSceneAOIModule::CreateSceneObject(const int nSceneID, const int nGroupID)
{
	return false;
}

int NFCSceneAOIModule::OnPropertyCommonEvent(const NFGUID & self, const std::string & strPropertyName, const NFIDataList::TData & oldVar, const NFIDataList::TData & newVar)
{
	if (NFrame::Player::GroupID() == strPropertyName)
	{
		OnGroupEvent(self, strPropertyName, oldVar, newVar);
	}

	if (NFrame::Player::SceneID() == strPropertyName)
	{
		OnSceneEvent(self, strPropertyName, oldVar, newVar);
	}

	
	if (NFrame::Player::ThisName() == m_pKernelModule->GetPropertyString(self, NFrame::Player::ClassName()))
	{
		NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->GetObject(self);
		if (xObject->GetState() != CLASS_OBJECT_EVENT::COE_CREATE_FINISH)
		{
			return 0;
		}
	}

	NFCDataList valueBroadCaseList;
	if (GetBroadCastObject(self, strPropertyName, false, valueBroadCaseList) <= 0)
	{
		return 0;
	}

	OnPropertyEvent(self, strPropertyName, oldVar, newVar, valueBroadCaseList);

	return 0;
}

int NFCSceneAOIModule::OnRecordCommonEvent(const NFGUID & self, const RECORD_EVENT_DATA & xEventData, const NFIDataList::TData & oldVar, const NFIDataList::TData & newVar)
{
	const std::string& strRecordName = xEventData.strRecordName;
	const int nOpType = xEventData.nOpType;
	const int nRow = xEventData.nRow;
	const int nCol = xEventData.nCol;

	int nObjectContainerID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
	int nObjectGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());

	if (nObjectGroupID < 0)
	{
		return 0;
	}

	if (NFrame::Player::ThisName() == m_pKernelModule->GetPropertyString(self, NFrame::Player::ClassName()))
	{
		NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->GetObject(self);
		if (xObject->GetState() != CLASS_OBJECT_EVENT::COE_CREATE_FINISH)
		{
			return 0;
		}
	}
	
	NFCDataList valueBroadCaseList;
	GetBroadCastObject(self, strRecordName, true, valueBroadCaseList);

	OnRecordEvent(self, strRecordName, xEventData, oldVar, newVar, valueBroadCaseList);

	return 0;
}

int NFCSceneAOIModule::OnClassCommonEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		const int nObjectSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
		const int nObjectGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::GroupID());

		if (nObjectGroupID < 0 || nObjectSceneID <= 0)
		{
			return 0;
		}

		NFCDataList valueAllObjectList;
		NFCDataList valueBroadCastList;
		NFCDataList valueBroadListNoSelf;

		m_pKernelModule->GetGroupObjectList(nObjectSceneID, nObjectGroupID, valueAllObjectList);

		for (int i = 0; i < valueAllObjectList.GetCount(); i++)
		{
			NFGUID identBC = valueAllObjectList.Object(i);
			const std::string& strClassName = m_pKernelModule->GetPropertyString(identBC, NFrame::IObject::ClassName());
			if (NFrame::Player::ThisName() == strClassName)
			{
				//yes, only boardcast to player
				valueBroadCastList.Add(identBC);
				if (identBC != self)
				{
					valueBroadListNoSelf.Add(identBC);
				}
			}
		}

		//tell other people that you want to leave from this scene or this group
		OnObjectListLeave(valueBroadListNoSelf, NFCDataList() << self);
	}

	else if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
	{

	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent)
	{
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
	{
		if (strClassName == NFrame::Player::ThisName())
		{
			//tell youself<client>, u want to enter this scene or this group
			OnObjectListEnter(NFCDataList() << self, NFCDataList() << self);

			//tell youself<client>, u want to broad your properties and records to youself
			OnPropertyEnter(NFCDataList() << self, self);
			OnRecordEnter(NFCDataList() << self, self);
		}
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{

	}

	return 0;
}

int NFCSceneAOIModule::OnGroupEvent(const NFGUID & self, const std::string & strPropertyName, const NFIDataList::TData & oldVar, const NFIDataList::TData & newVar)
{
	//this event only happened in the same group
	int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
	int nOldGroupID = oldVar.GetInt();
	if (nOldGroupID > 0)
	{
		//jump to 0 group from this group<nOldGroupID>
		NFCDataList valueAllOldObjectList;
		NFCDataList valueAllOldPlayerList;
		m_pKernelModule->GetGroupObjectList(nSceneID, nOldGroupID, valueAllOldObjectList);
		if (valueAllOldObjectList.GetCount() > 0)
		{
			for (int i = 0; i < valueAllOldObjectList.GetCount(); i++)
			{
				NFGUID identBC = valueAllOldObjectList.Object(i);

				if (valueAllOldObjectList.Object(i) == self)
				{
					valueAllOldObjectList.Set(i, NFGUID());
				}

				const std::string& strClassName = m_pKernelModule->GetPropertyString(identBC, NFrame::IObject::ClassName());
				if (NFrame::Player::ThisName() == strClassName)
				{
					valueAllOldPlayerList.Add(identBC);
				}
			}

			OnObjectListLeave(valueAllOldPlayerList, NFCDataList() << self);
			OnObjectListLeave(NFCDataList() << self, valueAllOldObjectList);
		}

		int nReason = AfterLeaveScene(self, nSceneID, nOldGroupID, 0, NFCDataList());
		if (nReason == 0)
		{
			m_pKernelModule->ReleaseGroupScene(nSceneID, nOldGroupID);
		}
	}

	int nNewGroupID = newVar.GetInt();
	if (nNewGroupID > 0)
	{
		//jump to this group<nNewGroupID> from 0 group
		NFCDataList valueAllObjectList;
		NFCDataList valueAllObjectListNoSelf;
		NFCDataList valuePlayerList;
		NFCDataList valuePlayerListNoSelf;
		m_pKernelModule->GetGroupObjectList(nSceneID, nNewGroupID, valueAllObjectList);
		for (int i = 0; i < valueAllObjectList.GetCount(); i++)
		{
			NFGUID identBC = valueAllObjectList.Object(i);
			const std::string& strClassName = m_pKernelModule->GetPropertyString(identBC, NFrame::IObject::ClassName());
			if (NFrame::Player::ThisName() == strClassName)
			{
				valuePlayerList.Add(identBC);
				if (identBC != self)
				{
					valuePlayerListNoSelf.Add(identBC);
				}
			}

			if (identBC != self)
			{
				valueAllObjectListNoSelf.Add(identBC);
			}
		}

		if (valuePlayerListNoSelf.GetCount() > 0)
		{
			OnObjectListEnter(valuePlayerListNoSelf, NFCDataList() << self);
		}

		const std::string& strSelfClassName = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());

		if (valueAllObjectListNoSelf.GetCount() > 0)
		{
			if (strSelfClassName == NFrame::Player::ThisName())
			{
				OnObjectListEnter(NFCDataList() << self, valueAllObjectListNoSelf);
			}
		}

		if (strSelfClassName == NFrame::Player::ThisName())
		{
			for (int i = 0; i < valueAllObjectListNoSelf.GetCount(); i++)
			{
				NFGUID identOld = valueAllObjectListNoSelf.Object(i);

				OnPropertyEnter(NFCDataList() << self, identOld);
				OnRecordEnter(NFCDataList() << self, identOld);
			}
		}

		if (valuePlayerListNoSelf.GetCount() > 0)
		{
			OnPropertyEnter(valuePlayerListNoSelf, self);
			OnRecordEnter(valuePlayerListNoSelf, self);
		}
	}

	return 0;
}

int NFCSceneAOIModule::OnSceneEvent(const NFGUID & self, const std::string & strPropertyName, const NFIDataList::TData & oldVar, const NFIDataList::TData & newVar)
{
	int nOldSceneID = oldVar.GetInt();
	int nNowSceneID = newVar.GetInt();

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "Enter Scene:", nNowSceneID);

	NFCDataList valueOldAllObjectList;
	NFCDataList valueNewAllObjectList;
	NFCDataList valueAllObjectListNoSelf;
	NFCDataList valuePlayerList;
	NFCDataList valuePlayerNoSelf;

	m_pKernelModule->GetGroupObjectList(nOldSceneID, 0, valueOldAllObjectList);
	m_pKernelModule->GetGroupObjectList(nNowSceneID, 0, valueNewAllObjectList);

	for (int i = 0; i < valueOldAllObjectList.GetCount(); i++)
	{
		NFGUID identBC = valueOldAllObjectList.Object(i);
		if (identBC == self)
		{
			valueOldAllObjectList.Set(i, NFGUID());
		}
	}

	for (int i = 0; i < valueNewAllObjectList.GetCount(); i++)
	{
		NFGUID identBC = valueNewAllObjectList.Object(i);
		const std::string& strClassName = m_pKernelModule->GetPropertyString(identBC, NFrame::IObject::ClassName());
		if (NFrame::Player::ThisName() == strClassName)
		{
			valuePlayerList.Add(identBC);
			if (identBC != self)
			{
				valuePlayerNoSelf.Add(identBC);
			}
		}

		if (identBC != self)
		{
			valueAllObjectListNoSelf.Add(identBC);
		}
	}

	//////////////////////////////////////////////////////////////////////////

	OnObjectListLeave(NFCDataList() << self, valueOldAllObjectList);

	if (valuePlayerList.GetCount() > 0)
	{
		OnObjectListEnter(valuePlayerNoSelf, NFCDataList() << self);
	}

	OnObjectListEnter(NFCDataList() << self, valueAllObjectListNoSelf);

	for (int i = 0; i < valueAllObjectListNoSelf.GetCount(); i++)
	{
		NFGUID identOld = valueAllObjectListNoSelf.Object(i);
		OnPropertyEnter(NFCDataList() << self, identOld);
		OnRecordEnter(NFCDataList() << self, identOld);
	}

	if (valuePlayerNoSelf.GetCount() > 0)
	{
		OnPropertyEnter(valuePlayerNoSelf, self);
		OnRecordEnter(valuePlayerNoSelf, self);
	}

	return 0;
}

int NFCSceneAOIModule::GetBroadCastObject(const NFGUID & self, const std::string & strPropertyName, const bool bTable, NFIDataList & valueObject)
{
	int nObjectContainerID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::SceneID());
	int nObjectGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::IObject::GroupID());

	const std::string& strClassName = m_pKernelModule->GetPropertyString(self, NFrame::IObject::ClassName());
	NF_SHARE_PTR<NFIRecordManager> pClassRecordManager = m_pClassModule->GetClassRecordManager(strClassName);
	NF_SHARE_PTR<NFIPropertyManager> pClassPropertyManager = m_pClassModule->GetClassPropertyManager(strClassName);

	NF_SHARE_PTR<NFIRecord> pRecord(NULL);
	NF_SHARE_PTR<NFIProperty> pProperty(NULL);
	if (bTable)
	{
		if (NULL == pClassRecordManager)
		{
			return -1;
		}

		pRecord = pClassRecordManager->GetElement(strPropertyName);
		if (NULL == pRecord)
		{
			return -1;
		}
	}
	else
	{
		if (NULL == pClassPropertyManager)
		{
			return -1;
		}
		pProperty = pClassPropertyManager->GetElement(strPropertyName);
		if (NULL == pProperty)
		{
			return -1;
		}
	}

	if (bTable)
	{
		if (pRecord->GetPublic())
		{
			m_pKernelModule->GetGroupObjectList(nObjectContainerID, nObjectGroupID, NFrame::Player::ThisName(), self, valueObject);
		}
		else if (pRecord->GetPrivate() && !pRecord->GetUpload())
		{//upload property can not board to itself
			valueObject.Add(self);
		}
	}
	else
	{
		if (pProperty->GetPublic())
		{
			m_pKernelModule->GetGroupObjectList(nObjectContainerID, nObjectGroupID, NFrame::Player::ThisName(), self, valueObject);
		}
		else if (pProperty->GetPrivate() && !pProperty->GetUpload())
		{
			//upload property can not board to itself
			valueObject.Add(self);
		}
	}

	return valueObject.GetCount();
}

int NFCSceneAOIModule::BeforeEnterScene(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList & argList)
{
	std::vector<BEFORE_ENTER_SCENE_FUNCTOR_PTR>::iterator it = mtBeforeEnterSceneCallback.begin();
	for (; it != mtBeforeEnterSceneCallback.end(); it++)
	{
		BEFORE_ENTER_SCENE_FUNCTOR_PTR& pFunPtr = *it;
		BEFORE_ENTER_SCENE_FUNCTOR* pFunc = pFunPtr.get();
		const int nReason = pFunc->operator()(self, nSceneID, nGroupID, nType, argList);
		if (nReason != 0)
		{
			return nReason;
		}
	}

	return 0;
}

int NFCSceneAOIModule::AfterEnterScene(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList & argList)
{
	std::vector<AFTER_ENTER_SCENE_FUNCTOR_PTR>::iterator it = mtBeforeEnterSceneCallback.begin();
	for (; it != mtBeforeEnterSceneCallback.end(); it++)
	{
		AFTER_ENTER_SCENE_FUNCTOR_PTR& pFunPtr = *it;
		AFTER_ENTER_SCENE_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, nSceneID, nGroupID, nType, argList);
	}

	return 0;
}

int NFCSceneAOIModule::BeforeLeaveScene(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList & argList)
{
	std::vector<BEFORE_LEAVE_SCENE_FUNCTOR_PTR>::iterator it = mtBeforeLeaveSceneCallback.begin();
	for (; it != mtBeforeLeaveSceneCallback.end(); it++)
	{
		BEFORE_LEAVE_SCENE_FUNCTOR_PTR& pFunPtr = *it;
		BEFORE_LEAVE_SCENE_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, nSceneID, nGroupID, nType, argList);
	}

	return 0;
}

int NFCSceneAOIModule::AfterLeaveScene(const NFGUID & self, const int nSceneID, const int nGroupID, const int nType, const NFIDataList & argList)
{
	std::vector<AFTER_LEAVE_SCENE_FUNCTOR_PTR>::iterator it = mtAfterLeaveSceneCallback.begin();
	for (; it != mtAfterLeaveSceneCallback.end(); it++)
	{
		AFTER_LEAVE_SCENE_FUNCTOR_PTR& pFunPtr = *it;
		AFTER_LEAVE_SCENE_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, nSceneID, nGroupID, nType, argList);
	}

	return 0;
}

int NFCSceneAOIModule::OnObjectListEnter(const NFIDataList & self, const NFIDataList & argVar)
{
	std::vector<OBJECT_ENTER_EVENT_FUNCTOR_PTR>::iterator it = mtObjectEnterCallback.begin();
	for (; it != mtObjectEnterCallback.end(); it++)
	{
		OBJECT_ENTER_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		OBJECT_ENTER_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, argVar);
	}

	return 0;
}

int NFCSceneAOIModule::OnObjectListLeave(const NFIDataList & self, const NFIDataList & argVar)
{
	std::vector<OBJECT_LEAVE_EVENT_FUNCTOR_PTR>::iterator it = mtObjectLeaveCallback.begin();
	for (; it != mtObjectLeaveCallback.end(); it++)
	{
		OBJECT_LEAVE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		OBJECT_LEAVE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, argVar);
	}

	return 0;
}

int NFCSceneAOIModule::OnPropertyEnter(const NFIDataList & argVar, const NFGUID & self)
{
	std::vector<PROPERTY_ENTER_EVENT_FUNCTOR_PTR>::iterator it = mtPropertyEnterCallback.begin();
	for (; it != mtPropertyEnterCallback.end(); it++)
	{
		PROPERTY_ENTER_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		PROPERTY_ENTER_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(argVar, self);
	}

	return 0;
}

int NFCSceneAOIModule::OnRecordEnter(const NFIDataList & argVar, const NFGUID & self)
{
	std::vector<RECORD_ENTER_EVENT_FUNCTOR_PTR>::iterator it = mtRecordEnterCallback.begin();
	for (; it != mtRecordEnterCallback.end(); it++)
	{
		RECORD_ENTER_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		RECORD_ENTER_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(argVar, self);
	}

	return 0;
}

int NFCSceneAOIModule::OnPropertyEvent(const NFGUID & self, const std::string & strProperty, const NFIDataList::TData & oldVar, const NFIDataList::TData & newVar, const NFIDataList& argVar)
{
	std::vector<PROPERTY_SINGLE_EVENT_FUNCTOR_PTR>::iterator it = mtPropertySingleCallback.begin();
	for (; it != mtPropertySingleCallback.end(); it++)
	{
		PROPERTY_SINGLE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		PROPERTY_SINGLE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, strProperty, oldVar, newVar, argVar);
	}

	return 0;
}

int NFCSceneAOIModule::OnRecordEvent(const NFGUID & self, const std::string& strProperty, const RECORD_EVENT_DATA & xEventData, const NFIDataList::TData & oldVar, const NFIDataList::TData & newVar, const NFIDataList& argVar)
{
	std::vector<RECORD_SINGLE_EVENT_FUNCTOR_PTR>::iterator it = mtRecordSingleCallback.begin();
	for (; it != mtRecordSingleCallback.end(); it++)
	{
		RECORD_SINGLE_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		RECORD_SINGLE_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(self, strProperty, xEventData, oldVar, newVar, argVar);
	}

	return 0;
}
