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

    LoadConfig();

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

            m_pEventProcessModule->AddEventCallBack(self, NFED_ON_OBJECT_ENTER_SCENE_RESULT, this, &NFCEctypeModule::OnEntrySceneEvent);
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

    int nContainerID = pObject->GetPropertyInt("SceneID");
    if (nContainerID <= 0)
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
        stream << "[ExitEctype] Account[" << strAccout << "] Level[" << nLevel << "] Scene[" << nContainerID << "] [0]";
        m_pLogModule->LogNormal(NFILogModule::NLL_DEBUG_NORMAL, self, stream, __FUNCTION__, __LINE__);
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, stream, __FUNCTION__, __LINE__);
        return 1;
    }

    if (!m_pSceneProcessModule->IsCloneScene(nContainerID))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "player not int clone scene", "", __FUNCTION__, __LINE__);
        return 1;
    }

    //std::string strSceneConfig = boost::lexical_cast<std::string>(nContainerID);
    //int nWinVp = m_pElementInfoModule->GetPropertyInt(strSceneConfig, "WinConsumeVP");
    //if (!m_pPropertyModule->ConsumeVP(self, nWinVp))
    //{
    //    m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "vp not enough", "", __FUNCTION__, __LINE__);
    //    return 1;
    //}

    ////----------------------------------------------------------------------------------------------
    //// anti plug-in, check ectype-scene finishing time and NPC HP
    //if (!CheckAntiPlugin(self, nContainerID))
    //{
    //    // 暂时先让继续通关
    //    //return 1;
    //}
    ////----------------------------------------------------------------------------------------------

    //m_pGSLogCollectModule->SetReason(self, NFDefine::TLogReasonType_EctypePass);
 //   AddEctypeAward(self, nContainerID, false);

 //   //任务事件
 //   NFCDataList xVar;
 //   int nTaskSceneType = m_pSceneProcessModule->GetCloneSceneType(nContainerID);
 //   //通关一次副本
 //   xVar << nContainerID  << nTaskSceneType << 1;
 //   m_pEventProcessModule->DoEvent(self, NFED_ON_TASK_PASS_CLONE, xVar);

 //   m_pKernelModule->SetPropertyInt(self, "AwardEnable", 0);

 //   CompleteEctypeMode(self, nContainerID, nStar);

	//m_pEventProcessModule->DoEvent(self, NFED_ON_TASK_PASS_CLONE_WITH_STAR, xVar << nStar);

 //   //扣除魔王梦靥次数
 //   std::string strSceneID = boost::lexical_cast<std::string>(nContainerID);
 //   if (m_pElementInfoModule->ExistElement(strSceneID))
 //   {
 //       E_SCENE_TYPE nSceneType = (E_SCENE_TYPE)m_pElementInfoModule->GetPropertyInt(strSceneID,"SceneType");
 //       m_pActivityWeeklyModule->DeductDevilLeftTime(self, nSceneType, strSceneID);
 //   }

    return 0;
}

bool NFCEctypeModule::CheckAntiPlugin(const NFIDENTID& self, const int nSceneID)
{
    //NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    //if (!pObject.get())
    //{
    //    m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "player not exit", "", __FUNCTION__, __LINE__);
    //    return false;
    //}

    //NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "EctypeList");
    //if (NULL == pRecord.get())
    //{
    //    return false;
    //}

    //NFCDataList xMatchList;
    //int nMatchCount = pRecord->FindInt(EXTYPE_RC_SCENEID, nSceneID, xMatchList);
    //if (nMatchCount != 1)
    //{
    //    m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "[Plug-in]not find scene", nSceneID, __FUNCTION__, __LINE__);
    //    return false;
    //}

    //std::string strSceneID = boost::lexical_cast<std::string>(nSceneID);
    //int nMinPassTime = m_pElementInfoModule->GetPropertyInt(strSceneID, "MinPassTime");

    //// 检查通关时间
    //int nStartTime = pRecord->GetInt(xMatchList.Int(0), EXTYPE_RC_TIME);
    //if (NFTimeEx::GetNowTime() - nStartTime < nMinPassTime )
    //{
    //    m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "[Plug-in]It's too quickly to pass ectype scene, cannot pass.", strSceneID, __FUNCTION__, __LINE__);
    //    return false;
    //}

    //// 检查副本中怪物血量
    //NF_SHARE_PTR<NFIRecord> pDamageRecord = m_pKernelModule->FindRecord(self, "NPCDamageList");
    //if (NULL == pDamageRecord.get())
    //{
    //    return false;
    //}

    //for (int i = 0; i < pDamageRecord->GetRows(); ++i)
    //{
    //    if (!pDamageRecord->IsUsed(i))
    //    {
    //        continue;
    //    }

    //    int nHP = pDamageRecord->GetInt(i, 1);
    //    if (nHP != 0)
    //    {
    //        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "[Plug-in]There's some alive NPC in ectype scene, cannot pass.", strSceneID, __FUNCTION__, __LINE__);
    //        return false;
    //    }
    //}

    return true;
}

int NFCEctypeModule::ExistActiveCloneScene(const NFIDENTID& self, const int nSceneID)
{
    return 0;
}

bool NFCEctypeModule::CompleteEctypeMode(const NFIDENTID self, const int nContainerID, const int nStar)
{
    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "EctypeList");
    if (NULL == pRecord.get())
    {
        return false;
    }

    NFCDataList valNormalResult;
    pRecord->FindInt(EXTYPE_RC_SCENEID, nContainerID, valNormalResult);

    if (valNormalResult.GetCount() != 1)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "not find scene", nContainerID, __FUNCTION__, __LINE__);
        return false;
    }

    std::ostringstream logStream;
    logStream << "CompleteEctypeMode for this Scene : " << nContainerID;
    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, logStream, __FUNCTION__, __LINE__);

    const int nFindRow = valNormalResult.Int(0);

    char szSceneIDName[MAX_PATH] = { 0 };
    sprintf(szSceneIDName, "%d", nContainerID);

    SceneInfo xSceneInfo;
    if(GetSceneInfo(szSceneIDName, xSceneInfo))
    {
        int nNextID;
        bool bRet = xSceneInfo.mListNextScene.First(nNextID);
        for (; bRet ; bRet = xSceneInfo.mListNextScene.Next(nNextID))
        {
            AddEctypeActiveState(self, nNextID);
        }
    }

    const int nStatus = pRecord->GetInt(nFindRow, EXTYPE_RC_STATE);

    int64_t nTime = NFTimeEx::GetNowTime();
    if (nStatus != ECS_HAS_PASS)
    {
        pRecord->SetInt(nFindRow, EXTYPE_RC_STATE, ECS_HAS_PASS);
    }

    int nOldStar = pRecord->GetInt(nFindRow, EXTYPE_RC_STAR);
    if (nStar > nOldStar)
    {
        pRecord->SetInt(nFindRow, EXTYPE_RC_STAR, nStar);
    }

    return true;
}

int NFCEctypeModule::OnObjectGroupIDEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar, const NFIDataList& argVar)
{

    return 0;
}

int NFCEctypeModule::AddEctypeAward(const NFIDENTID& self, const int nSceneID, const bool bWipe)
{
    std::string strSceneID = boost::lexical_cast<std::string>(nSceneID);
    int nType = m_pElementInfoModule->GetPropertyInt(strSceneID, "SceneType");

    // TODO:过关了，把DropItemList的所有东西都领了
    // 同时把Scene里SceneAward PassAward WipeAward三个奖励也给了
    // 注意WipeAward只有在bWipe的情况下才给，注意nWipeCount扫荡次数

    NF_SHARE_PTR<NFIRecord> pDropRecord = m_pKernelModule->FindRecord(self, "DropItemList");
    if (NULL == pDropRecord.get())
    {
        return 0;
    }

  //  for (int i = 0; i < pDropRecord->GetRows(); i++)
  //  {
  //      if (!pDropRecord->IsUsed(i))
  //      {
  //          break;
  //      }

  //      const std::string& strItemConfig = pDropRecord->GetString(i, NFIPackModule::DropItemID);
  //      if (strItemConfig.empty())
  //      {
  //          continue;
  //      }

  //      if ((pDropRecord->GetInt(i, NFIPackModule::DropValid) == 1) &&
  //          pDropRecord->GetInt(i, NFIPackModule::DropPick) == 0)
  //      {
  //          int nCount = pDropRecord->GetInt(i, NFIPackModule::DropCount);
  //          m_pPackModule->CreateItem(self, strItemConfig, EGIET_NONE, nCount);

  //          pDropRecord->SetInt(i , NFIPackModule::DropPick, 1);
  //      }

  //  }

  //  const std::string& strSceneAward =  m_pElementInfoModule->GetPropertyString(szSceneIDName, "SceneAward");
  //  if (!strSceneAward.empty())
  //  {
  //      m_pAwardModule->DoAward(self, strSceneAward);
  //  }


  //  SceneInfo xSceneInfo;
  //  if(GetSceneInfo(szSceneIDName, xSceneInfo))
  //  {
  //      NFCDataList xItemList;
  //      NFCDataList xCountList;
  //      NFCDataList xVar;

  //      std::string strPassAward;
  //      bool bRet = xSceneInfo.mListPassAward.First(strPassAward);
  //      for (; bRet && !strPassAward.empty(); bRet = xSceneInfo.mListPassAward.Next(strPassAward))
  //      {
  //          m_pAwardModule->DoAward(self, strPassAward, NFDefine::Mail_None, xItemList, xCountList);
  //      }

  //      NFCDataList xResult;
		//xResult << SCENE_AWARD_TYPE_NORMAL;
  //      for (int i = 0; i < xItemList.GetCount() && i < xCountList.GetCount(); i++)
  //      {
  //          xResult << xItemList.String(i) << xCountList.Int(i);
  //      }

  //      m_pEventProcessModule->DoEvent(self, EFID_ON_SCENE_AWARD_RESULT, xResult);
  //  }

  //  //非扫荡给经验奖励
  //  const NFINT64 nHeroExp =  m_pElementInfoModule->GetPropertyInt(szSceneIDName, "HeroAward");
  //  if (!bWipe && nHeroExp > 0)
  //  {
  //      int nTroopType = m_pElementInfoModule->GetPropertyInt(szSceneIDName, "TroopType");
  //      m_pHeroModule->AddTroopHeroExp(self, nTroopType, nHeroExp);
  //  }

  //  if (bWipe)
  //  {
  //      const std::string& strWipeAward =  m_pElementInfoModule->GetPropertyString(szSceneIDName, "WipeAward");
  //      if (!strWipeAward.empty())
  //      {
  //          m_pAwardModule->DoAward(self, strWipeAward);
  //      }
  //  }

    return 0;
}

bool NFCEctypeModule::AddNewEctype(const NFIDENTID self)
{
    int nLevel = m_pKernelModule->GetPropertyInt(self, "Level");
    //int id = m_pEquipConfigModule->GetPlayerInitIndex(self);

    //NFCDataList var;
    //if (m_pEquipConfigModule->QueryInitOpenScene(id, nLevel, var))
    //{
    //    for (int i = 0; i < var.GetCount(); ++i)
    //    {
    //        AddEctypeActiveState(self, var.Int(i));
    //    }
    //}

    return true;
}

int NFCEctypeModule::OnWipeScene( const NFIDENTID& self, int nWipeCount, int nScene )
{
 //   const std::string strSceneConfig = boost::lexical_cast<std::string> (nScene);
 //   const std::string& strWipeItem = m_pElementInfoModule->GetPropertyString(strSceneConfig, "WipeItem");
 //   if (strWipeItem.empty() || !m_pElementInfoModule->ExistElement(strWipeItem))
 //   {
 //       m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "strWipeItem error", "", __FUNCTION__, __LINE__);
 //       return 0;
 //   }

 //   int nWipeItemCount = m_pElementInfoModule->GetPropertyInt(strSceneConfig, "WipeItemCount");

 //   NF_SHARED_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "EctypeList");
 //   if (NULL == pRecord.get())
 //   {
 //       return 0;
 //   }

 //   NFCDataList valNormalResult;
 //   pRecord->FindInt(EXTYPE_RC_SCENEID, nScene, valNormalResult);

 //   if (valNormalResult.GetCount() != 1)
 //   {
 //       m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "wipescene not find scene", nScene, __FUNCTION__, __LINE__);
 //       return 0;
 //   }

 //   int nVPConsume = m_pElementInfoModule->GetPropertyInt(strSceneConfig, "ConsumeVP");
 //   int nNeedVP = nVPConsume * nWipeCount;
 //   if (!m_pPropertyModule->EnoughVP(self, nNeedVP ))
 //   {
 //       m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "wipescene not enoughVP", nScene, __FUNCTION__, __LINE__);
 //       return 0;
 //   }

 //   //3星扫荡
 //   int nRow = valNormalResult.Int(0);
 //   if (pRecord->GetInt(nRow, EXTYPE_RC_STAR) != 3)
 //   {
 //       m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "wipescene star error", nScene, __FUNCTION__, __LINE__);
 //       return 0;
 //   }

 //   int nSceneType = m_pSceneProcessModule->GetCloneSceneType(nScene);
 //   if (nSceneType == SCENE_TYPE_ELITE)
 //   {
 //       const int nTodayCount = pRecord->GetInt(nRow, EXTYPE_RC_TODAY_COUNT);
 //       const int nLimitTimes = m_pElementInfoModule->GetPropertyInt(strSceneConfig, "TimesLimit");
 //       if(nWipeCount > nLimitTimes - nTodayCount)
 //       {
 //           m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "wipescene times limit", nScene, __FUNCTION__, __LINE__);
 //           return -1;
 //       }
 //   }

 //   if (nWipeCount > 1)
 //   {
 //       if (!m_pGameFunctionModule->IsFunctionOpen(self, NFIGameFunctionModule::GAMEFUNCTION_SCAN10))
 //       {
 //           m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "GAMEFUNCTION_SCAN10 Function is Not Open", NFIGameFunctionModule::GAMEFUNCTION_SCAN10, __FUNCTION__, __LINE__);
 //           return -1;
 //       }
 //   }

 //   int nItemCount = m_pPackModule->QueryCount(self, strWipeItem);
 //   //道具不够扣元宝逻辑
 //   if (nWipeCount * nWipeItemCount > nItemCount)
 //   {
 //       int nNeed = nWipeCount * nWipeItemCount - nItemCount;
 //       int nPrice = nNeed * m_pElementInfoModule->GetPropertyInt(strSceneConfig, "WipeCost"); 
 //       if (nPrice <= 0)
 //       {
 //           m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "price error", nScene, __FUNCTION__, __LINE__);
 //           return -1;
 //       }

 //       if (!m_pGameFunctionModule->IsFunctionOpen(self, NFIGameFunctionModule::GAMEFUNCTION_YBPSCANCARD))
 //       {
 //           m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "GAMEFUNCTION_YBPSCANCARD Function is Not Open", NFIGameFunctionModule::GAMEFUNCTION_YBPSCANCARD, __FUNCTION__, __LINE__);
 //           return -1;
 //       }


 //       NFIPropertyModule::ConsumeInfo xConsumeInfo;
 //       xConsumeInfo.nCurrencyType = NFDefine::Currency_YB;
 //       xConsumeInfo.nCurrencyValue = nPrice;
 //       xConsumeInfo.nPayType = NFDefine::TXPayType_Wipe_Scene;
 //       xConsumeInfo.nReason = NFDefine::TLogReasonType_Wipe_Scene;
 //       xConsumeInfo.strPayItemID = strSceneConfig;
 //       xConsumeInfo.strPayItemName = strSceneConfig;
 //       xConsumeInfo.nItemCount = nWipeCount;
 //       xConsumeInfo.nPayeItem_Price = nItemCount;
 //       xConsumeInfo.strAppRemark = __FUNCTION__;

 //       NFMsg::ReqWipeScene xMsg;
 //       *xMsg.mutable_player_id() = NFIdentToPBIdent(self);
 //       xMsg.set_scene_id(nScene);
 //       xMsg.set_wipe_count(nWipeCount);

 //       std::string strParm;
 //       xMsg.SerializeToString(&strParm);

 //       xConsumeInfo.strSomeParam = strParm;
 //       m_pPropertyModule->ConsumeCurrency(self, xConsumeInfo);

 //       //退出进入元宝消费逻辑
 //       return 0;
 //   }

 //   if (nSceneType == SCENE_TYPE_ELITE)
 //   {
 //       int nTodayCount = pRecord->GetInt(nRow, EXTYPE_RC_TODAY_COUNT);
 //       pRecord->SetInt(nRow, EXTYPE_RC_TODAY_COUNT, nTodayCount + nWipeCount);

 //   }

 //   if (!m_pPackModule->DeleteItem(self, strWipeItem, nWipeItemCount * nWipeCount))
 //   {
 //       m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "wipescene item not enough", nScene, __FUNCTION__, __LINE__);

 //       return 0;
 //   }

 //   m_pPropertyModule->ConsumeVP(self, nNeedVP);

 //   for (int i = 0; i < nWipeCount; i++)
 //   {
 //       AddEctypeAward(self, nScene, true);
 //   }


 //   NFCDataList xVar;
 //   int nTaskSceneType = m_pSceneProcessModule->GetCloneSceneType(nScene);
 //   xVar << nScene  << nTaskSceneType << nWipeCount;
 //   m_pEventProcessModule->DoEvent(self, NFED_ON_TASK_PASS_CLONE, xVar);
	//m_pEventProcessModule->DoEvent(self, NFED_ON_WIPE_SCENE, xVar);

    return 0;
}

int NFCEctypeModule::OnEntrySceneEvent( const NFIDENTID& self, const int nEventID, const NFIDataList& var )
{
    //if (var.GetCount() != 8  ||
    //    !var.TypeEx(TDATA_OBJECT, TDATA_INT, TDATA_INT, TDATA_INT, TDATA_INT, TDATA_FLOAT, TDATA_FLOAT, TDATA_FLOAT, TDATA_UNKNOWN))
    //{
    //    m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "param error", nEventID, __FUNCTION__, __LINE__);
    //    return 1;
    //}

    //NF_SHARED_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    //if (!pObject.get())
    //{
    //    m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "player not exit", "", __FUNCTION__, __LINE__);
    //    return 0;
    //}

    //int nSceneId = var.Int(2);//目标场景ID

    //if (m_pSceneProcessModule->IsCloneScene(nSceneId))
    //{
    //    m_pKernelModule->SetPropertyInt(self, "AwardEnable", 1);
    //    // TODO::设置副本进入时间
    //    NF_SHARED_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "EctypeList");
    //    if (NULL == pRecord.get())
    //    {
    //        return 1;
    //    }

    //    int nContainerID = pObject->GetPropertyInt("SceneID");
    //    NFCDataList xMatchList;
    //    int nMatchCount = pRecord->FindInt(EXTYPE_RC_SCENEID, nContainerID, xMatchList);
    //    if (nMatchCount != 1)
    //    {
    //        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "scene error", nContainerID, __FUNCTION__, __LINE__);
    //        return 1;
    //    }

    //    pRecord->SetInt(xMatchList.Int(0), EXTYPE_RC_TIME, NFTimeEx::GetNowTime());

    //    const std::string& strAccount = m_pKernelModule->GetPropertyString(self, "Account");
    //    int nLevel = m_pKernelModule->GetPropertyInt(self, "Level");

    //    std::ostringstream stream;
    //    stream << "[EntryEctype] Account[" << strAccount << "] Level[" << nLevel << "] Scene[" << nSceneId << "]";
    //    m_pLogModule->LogNormal(NFILogModule::NLL_DEBUG_NORMAL, self, stream, __FUNCTION__, __LINE__);
    //}
    //else
    //{
    //    m_pKernelModule->SetPropertyInt(self, "AwardEnable", 0);
    //}

    return 0;
}

int NFCEctypeModule::ClearSceneTime(const NFIDENTID& self)
{
    //NF_SHARED_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, "EctypeList");
    //if (!pRecord.get())
    //{
    //    return 0;
    //}

    //int nRows = pRecord->GetRows();
    //for(int i = 0; i < nRows ; i++)
    //{
    //    if (!pRecord->IsUsed(i))
    //    {
    //        continue;
    //    }

    //    int nCount = pRecord->GetInt(i, EXTYPE_RC_TODAY_COUNT);
    //    if (nCount > 0)
    //    {
    //        pRecord->SetInt(i, EXTYPE_RC_TODAY_COUNT, 0);
    //        pRecord->SetInt(i, EXTYPE_RC_BUY_COUNT, 0);
    //    }
    //}

    return 0;
}

int NFCEctypeModule::LoadConfig()
{
    ClearConfig();

    //NF_SHARED_PTR<NFILogicClass> pClass = m_pLogicClassModule->GetElement("SceneReset");
    //if (pClass.get() == NULL)
    //{
    //    NFASSERT(0, "SceneReset error", __FILE__, __FUNCTION__);
    //}

    //m_mapBuyElite.clear();

    //NFList<std::string>& xConfigList = pClass->GetConfigNameList();
    //std::string strConfigId;
    //for (bool bRet = xConfigList.First(strConfigId); bRet; bRet = xConfigList.Next(strConfigId))
    //{
    //    if (!m_pElementInfoModule->ExistElement(strConfigId))
    //    {
    //        NFASSERT(0, "configId error", __FILE__, __FUNCTION__);
    //    }

    //    int nNum = m_pElementInfoModule->GetPropertyInt(strConfigId, "Num");
    //    if (nNum <= 0)
    //    {
    //        NFASSERT(0, "SceneReset num error", __FILE__, __FUNCTION__);
    //    }

    //    m_mapBuyElite[nNum] = strConfigId;
    //}

    //NF_SHARED_PTR<NFILogicClass> pLogicCLass = m_pLogicClassModule->GetElement("Scene");
    //if (!pLogicCLass.get())
    //{
    //    NFASSERT(0, "Scene error", __FILE__, __FUNCTION__);
    //}

    //NFList<std::string>& configList = pLogicCLass->GetConfigNameList();
    //std::string strSceneConfigID;
    //for (bool bRet = configList.First(strSceneConfigID); bRet; bRet = configList.Next(strSceneConfigID))
    //{
    //    SceneInfo xSceneInfo;
    //    const std::string& strNextScene = m_pElementInfoModule->GetPropertyString(strSceneConfigID, "NextSceneID");
    //    NFCDataList VarNext;
    //    VarNext.Split(strNextScene.c_str(), ",");
    //    for(int i = 0; i < VarNext.GetCount(); i++)
    //    {
    //        const std::string& strNeID = VarNext.String(i);
    //        if (strNeID.empty())
    //        {
    //            continue;
    //        }

    //        if (m_pElementInfoModule->ExistElement(strNeID))
    //        {
    //            int nValue = 0;
    //            if(!NF_StrTo(strNeID, nValue))
    //            {
    //                NFASSERT(0 , strNeID, __FILE__, __FUNCTION__);
    //            }

    //            xSceneInfo.mListNextScene.Add(nValue);
    //        }

    //    }

    //    const std::string& strPassAward = m_pElementInfoModule->GetPropertyString(strSceneConfigID, "PassAward");

    //    VarNext.Clear();
    //    VarNext.Split(strPassAward.c_str(), ",");
    //    for(int i = 0; i < VarNext.GetCount(); i++)
    //    {
    //        const std::string& strNeID = VarNext.String(i);
    //        if (strNeID.empty())
    //        {
    //            continue;
    //        }

    //        if (!m_pAwardModule->ExistAawardPack(strNeID))
    //        {
    //            printf("%s \n", strNeID.c_str());
    //            NFASSERT(0 , strNeID, __FILE__, __FUNCTION__);
    //        }

    //        xSceneInfo.mListPassAward.Add(strNeID);
    //    }

    //    m_mapSceneInfo.insert(std::make_pair(strSceneConfigID, xSceneInfo));

    //    const std::string& strSceneAward = m_pElementInfoModule->GetPropertyString(strSceneConfigID, "SceneAward");
    //    if (!strSceneAward.empty() && !m_pAwardModule->ExistAawardPack(strSceneAward))
    //    {
    //         printf("%s \n", strSceneAward.c_str());
    //         NFASSERT(0 , strSceneAward, __FILE__, __FUNCTION__);
    //    }

    //    const std::string& strWipeAward = m_pElementInfoModule->GetPropertyString(strSceneConfigID, "WipeAward");
    //    if (!strWipeAward.empty() && !m_pAwardModule->ExistAawardPack(strWipeAward))
    //    {
    //         printf("%s \n", strWipeAward.c_str());
    //         NFASSERT(0 , strWipeAward, __FILE__, __FUNCTION__);
    //    }

    //}

    return 0;
}

bool NFCEctypeModule::GetSceneInfo(const std::string& strConfig, SceneInfo& xSceneInfo)
{
    std::map<std::string, SceneInfo>::iterator itr = m_mapSceneInfo.find(strConfig);
    if (itr == m_mapSceneInfo.end())
    {
        return false;
    }

    xSceneInfo = itr->second;
    return true;
}

void NFCEctypeModule::ClearConfig()
{
    m_mapSceneInfo.clear();
    m_mapBuyElite.clear();
}
