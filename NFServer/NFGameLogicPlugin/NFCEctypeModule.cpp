// -------------------------------------------------------------------------
//    @FileName         ：    NFCEctypeModule.cpp
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2012-12-15
//    @Module           ：    NFCEctypeModule
//
// -------------------------------------------------------------------------

#include "NFCEctypeModule.h"
#include "NFComm/NFCore/NFTimer.h"
#include "NFComm/Define/NFStringInfo.h"

bool NFCEctypeModule::Init()
{
    return true;
}

bool NFCEctypeModule::Shut()
{
    return true;
}

bool NFCEctypeModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

bool NFCEctypeModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pSceneProcessModule = dynamic_cast<NFISceneProcessModule*>(pPluginManager->FindModule("NFCSceneProcessModule"));
    m_pAwardModule = dynamic_cast<NFIAwardPackModule*>(pPluginManager->FindModule("NFCAwardPackModule"));
    m_pPackModule = dynamic_cast<NFIPackModule*>(pPluginManager->FindModule("NFCPackModule"));
    m_pPropertyModule = dynamic_cast<NFIPropertyModule*>(pPluginManager->FindModule("NFCPropertyModule"));
    m_pLevelModule = dynamic_cast<NFILevelModule*>(pPluginManager->FindModule("NFCLevelModule"));
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));

    assert(NULL != m_pLevelModule);
    assert(NULL != m_pPropertyModule);
    assert(NULL != m_pPackModule);
    assert(NULL != m_pAwardModule);
    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pSceneProcessModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pLogicClassModule);

    m_pEventProcessModule->AddClassCallBack("Player", this, &NFCEctypeModule::OnObjectClassEvent);

    return true;
}

bool NFCEctypeModule::CanEntryCloneScene(const NFIDENTID self, const int nContainerID)
{
    std::string strSceneID = boost::lexical_cast<std::string>(nContainerID);
    //如果没找到，就判断是否是第一个场景IsFirstCloneScene
    int nLevel = m_pKernelModule->GetPropertyInt(self, "Level");
    if (nLevel < m_pElementInfoModule->GetPropertyInt(strSceneID, "SceneLevelLimit"))
    {
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self,  "Level limit for scene:" + strSceneID, nLevel, __FUNCTION__, __LINE__);
        return false;
    }

    //更改后则直接看是否存在
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "EctypeList");
    if (!pRecord.get())
    {
        return false;
    }

    NFCDataList valNormalResult;
    pRecord->FindInt(EXTYPE_RC_SCENEID, nContainerID, valNormalResult);
    if (valNormalResult.GetCount() <= 0)
    {
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "no this scene in record", nContainerID, __FUNCTION__, __LINE__);
        return false;
    }

    return true;
}

int NFCEctypeModule::OnObjectClassEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    if (strClassName == "Player")
    {
        if (COE_CREATE_BEFORE_EFFECT == eClassEvent)
        {
            int nOnlineCount = m_pKernelModule->GetPropertyInt(self, "OnlineCount");
            if (nOnlineCount <= 0)
            {
                NF_SHARE_PTR<NFILogicClass> pLogicCLass = m_pLogicClassModule->GetElement("Scene");
                if (!pLogicCLass.get())
                {
                   return 0;
                }

                NFList<std::string>& configList = pLogicCLass->GetConfigNameList();
                std::string strSceneConfigID;
                for (bool bRet = configList.First(strSceneConfigID); bRet; bRet = configList.Next(strSceneConfigID))
                {
                    int nFirstScene = m_pElementInfoModule->GetPropertyInt(strSceneConfigID, "IsFirstCloneScene");
                    //第一关，并且是普通模式才可以，其他模式要求普通模式开启才行
                    if (nFirstScene > 0)
                    {
                        int nSceneId = 0;
                        if(NF_StrTo(strSceneConfigID, nSceneId))
                        {//免激活的地图直接可以进，这里手动激活一下，否则EctypeList里没有数据
                            AddEctypeActiveState(self, nSceneId);
                        }
                    }
                }
            }
        }
        else if (COE_CREATE_FINISH == eClassEvent)
        {
            m_pKernelModule->AddPropertyCallBack(self, "GroupID", this, &NFCEctypeModule::OnObjectGroupIDEvent);

            //m_pEventProcessModule->AddEventCallBack(self, NFED_ON_OBJECT_ENTER_SCENE_RESULT, this, &NFCEctypeModule::OnEntrySceneEvent);
        }
    }
    return 0;
}

bool NFCEctypeModule::AddEctypeActiveState(const NFIDENTID self, const int nContainerID)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "EctypeList");
    if (pRecord.get() && nContainerID > 0)
    {
        NFCDataList valNormalResult;
        pRecord->FindInt(EXTYPE_RC_SCENEID, nContainerID, valNormalResult);
        if (valNormalResult.GetCount()  <= 0)
        {
            NFCDataList valValue;
            valValue.AddInt(nContainerID);
            valValue.AddInt(ECS_HAS_OPEN);
            valValue.AddInt(0);

            pRecord->AddRow(-1, valValue);
            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "EctypeList AddEctypeActiveState", nContainerID);
        }
    }

    return true;
}

int NFCEctypeModule::OnEctypeSettleEvent(const NFIDENTID& self, int nResult, int nStar)
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (!pObject.get())
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "player not exit", "", __FUNCTION__, __LINE__);
        return 1;
    }

    int nSceneID = pObject->GetPropertyInt("SceneID");
    if (nSceneID <= 0)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "scene id error", "", __FUNCTION__, __LINE__);
        return 1;
    }

    // 失败
    if (nResult == 0)
    {
        int nLevel = m_pKernelModule->GetPropertyInt(self, "Level");
        const std::string& strAccout = m_pKernelModule->GetPropertyString(self, "Account");

        std::ostringstream stream;
        stream << "[ExitEctype] Account[" << strAccout << "] Level[" << nLevel << "] Scene[" << nSceneID << "] [0]";
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, stream, __FUNCTION__, __LINE__);
        return 1;
    }

    if (!m_pSceneProcessModule->IsCloneScene(nSceneID))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "player not int clone scene", "", __FUNCTION__, __LINE__);
        return 1;
    }

    // 通关记录
    // TODO

    // 通关奖励
    AddEctypeAward(self, nSceneID);

    return 0;
}

bool NFCEctypeModule::CompleteEctypeMode(const NFIDENTID self, const int nContainerID, const int nStar)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "EctypeList");
    if (NULL == pRecord.get())
    {
        return false;
    }

    //

    return true;
}

int NFCEctypeModule::OnObjectGroupIDEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar)
{

    return 0;
}

int NFCEctypeModule::AddEctypeAward(const NFIDENTID& self, const int nSceneID)
{
    std::string strSceneID = boost::lexical_cast<std::string>(nSceneID);
    int nType = m_pElementInfoModule->GetPropertyInt(strSceneID, "SceneType");

    int nAddMoney = 0;
	int nAddExp = 0;
	int nAddDiamnod = 0;
    NFCDataList xItemList;
    NFCDataList xCountList;

    // 掉落奖励
    m_pPackModule->DrawDropAward(self, nAddMoney, nAddExp, xItemList, xCountList);

    // 通关奖励

    // 通知客户端
    NFCDataList xAwardInfoList;
    xAwardInfoList << nAddMoney;
	xAwardInfoList << nAddExp;
	xAwardInfoList << nAddDiamnod;

    if (xItemList.GetCount() == xCountList.GetCount())
    {
        for (int i = 0; i < xItemList.GetCount(); ++i)
        {
            xAwardInfoList << xItemList.String(i);
            xAwardInfoList << xCountList.Int(i);
        }
    }

    m_pEventProcessModule->DoEvent(self, NFED_ON_NOTICE_ECTYPE_AWARD, xAwardInfoList);

    return 0;
}

bool NFCEctypeModule::AddNewEctype(const NFIDENTID self)
{
    int nLevel = m_pKernelModule->GetPropertyInt(self, "Level");

    //

    return true;
}