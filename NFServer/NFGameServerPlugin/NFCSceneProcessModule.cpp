// -------------------------------------------------------------------------
//    @FileName      :    NFCSceneProcessModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-04-14
//    @Module           :    NFCSceneProcessModule
//
// -------------------------------------------------------------------------

#include "NFCSceneProcessModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFCore/NFTimer.h"

bool NFCSceneProcessModule::Init()
{
    return true;
}

bool NFCSceneProcessModule::Shut()
{
    return true;
}

bool NFCSceneProcessModule::Execute()
{
    return true;
}

bool NFCSceneProcessModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>("NFCGameServerNet_ServerModule");

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pGameServerNet_ServerModule);

    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCSceneProcessModule::OnObjectClassEvent);
    //////////////////////////////////////////////////////////////////////////

    //��ʼ����������
    // #ifdef NF_USE_ACTOR
    //  int nSelfActorID = pPluginManager->GetActorID();
    // #endif
    NF_SHARE_PTR<NFILogicClass> pLogicClass =  m_pLogicClassModule->GetElement("Scene");
    if (pLogicClass.get())
    {
        NFList<std::string>& list = pLogicClass->GetConfigNameList();

        std::string strData;
        bool bRet = list.First(strData);
        while (bRet)
        {
            int nSceneID = lexical_cast<int>(strData);

            LoadSceneResource(nSceneID);

            m_pKernelModule->CreateScene(nSceneID);

            bRet = list.Next(strData);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    // add msg handler
    if (!m_pGameServerNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_SWAP_SCENE, this, &NFCSceneProcessModule::OnClienSwapSceneProcess))
    {
        return false;
    }

    return true;
}

bool NFCSceneProcessModule::CreateSceneObject(const int nSceneID, const int nGroupID)
{
    NF_SHARE_PTR<NFMapEx<std::string, SceneSeedResource>> pSceneResource = mtSceneResourceConfig.GetElement(nSceneID);
    if (pSceneResource.get())
    {
        NF_SHARE_PTR<SceneSeedResource> pResource = pSceneResource->First();
        while (pResource.get())
        {
            const std::string& strClassName = m_pElementInfoModule->GetPropertyString(pResource->strConfigID, NFrame::NPC::ClassName());

            NFCDataList arg;
            arg << NFrame::NPC::X() << pResource->fSeedX;
            arg << NFrame::NPC::Y() << pResource->fSeedY;
            arg << NFrame::NPC::Z() << pResource->fSeedZ;
            arg << NFrame::NPC::SeedID() << pResource->strSeedID;

            m_pKernelModule->CreateObject(NFGUID(), nSceneID, nGroupID, strClassName, pResource->strConfigID, arg);

            pResource = pSceneResource->Next();
        }
    }

    return true;
}

int NFCSceneProcessModule::CreateCloneScene(const int& nSceneID)
{
    const E_SCENE_TYPE eType = GetCloneSceneType(nSceneID);
    int nTargetGroupID = m_pKernelModule->RequestGroupScene(nSceneID);

    if (nTargetGroupID > 0 && eType == SCENE_TYPE_CLONE_SCENE)
    {
        CreateSceneObject(nSceneID, nTargetGroupID);
    }

    return nTargetGroupID;
}

int NFCSceneProcessModule::OnEnterSceneEvent(const NFGUID& self, const int nEventID, const NFIDataList& var)
{
    if (var.GetCount() != 4
        || !var.TypeEx(TDATA_TYPE::TDATA_OBJECT, TDATA_TYPE::TDATA_INT,
                       TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return 0;
    }

    const NFGUID ident = var.Object(0);
    const int nType = var.Int(1);
    const int nTargetScene = var.Int(2);
    const int nTargetGroupID = var.Int(3);
    const int nNowSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
    const int nNowGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());

    if (self != ident)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, ident, "you are not you self, but you want to entry this scene", nTargetScene);
        return 1;
    }

    if (nNowSceneID == nTargetScene
        && nTargetGroupID == nNowGroupID)
    {
        //���������������������ͱ��л���
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "in same scene and group but it not a clone scene", nTargetScene);

        return 1;
    }

    //ÿ����ң�һ������
    NFINT64 nNewGroupID = 0;
    if (nTargetGroupID <= 0)
    {
        nNewGroupID = CreateCloneScene(nTargetScene);
    }
    else
    {
        nNewGroupID = nTargetGroupID;
    }

    if (nNewGroupID <= 0)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "CreateCloneScene failed", nTargetScene);
        return 0;
    }

    //�õ�����
    double fX = 0.0;
    double fY = 0.0;
    double fZ = 0.0;
    const std::string strSceneID = lexical_cast<std::string>(nTargetScene);
    const std::string& strRelivePosList = m_pElementInfoModule->GetPropertyString(strSceneID, NFrame::Scene::RelivePos());

    NFCDataList valueRelivePosList(strRelivePosList.c_str(), ";");
    if (valueRelivePosList.GetCount() >= 1)
    {
        NFCDataList valueRelivePos(valueRelivePosList.String(0).c_str(), ",");
        if (valueRelivePos.GetCount() == 3)
        {
            fX = lexical_cast<double>(valueRelivePos.String(0));
            fY = lexical_cast<double>(valueRelivePos.String(1));
            fZ = lexical_cast<double>(valueRelivePos.String(2));
        }
    }

    NFCDataList xSceneResult(var);
    xSceneResult.Add(fX);
    xSceneResult.Add(fY);
    xSceneResult.Add(fZ);

    m_pKernelModule->DoEvent(self, NFED_ON_OBJECT_ENTER_SCENE_BEFORE, xSceneResult);

    if (!m_pKernelModule->SwitchScene(self, nTargetScene, nNewGroupID, fX, fY, fZ, 0.0f, var))
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "SwitchScene failed", nTargetScene);

        return 0;
    }

    xSceneResult.Add(nNewGroupID);
    m_pKernelModule->DoEvent(self, NFED_ON_OBJECT_ENTER_SCENE_RESULT, xSceneResult);

    return 0;
}

int NFCSceneProcessModule::OnLeaveSceneEvent(const NFGUID& object, const int nEventID, const NFIDataList& var)
{
    if (1 != var.GetCount()
        || !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return -1;
    }

    NFINT32 nOldGroupID = var.Int(0);
    if (nOldGroupID > 0)
    {
        int nSceneID = m_pKernelModule->GetPropertyInt(object, NFrame::Player::SceneID());
        if (GetCloneSceneType(nSceneID) == SCENE_TYPE_CLONE_SCENE)
        {
            m_pKernelModule->ReleaseGroupScene(nSceneID, nOldGroupID);

            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, object, "DestroyCloneSceneGroup", nOldGroupID);
        }
    }

    return 0;
}

int NFCSceneProcessModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
    if (strClassName == "Player")
    {
        if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
        {
            //����ڸ�����,��ɾ�������Ǹ�����
            int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
            if (GetCloneSceneType(nSceneID) == SCENE_TYPE_CLONE_SCENE)
            {
                int nGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());

                m_pKernelModule->ReleaseGroupScene(nSceneID, nGroupID);

                m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "DestroyCloneSceneGroup", nGroupID);

            }
        }
        else if (CLASS_OBJECT_EVENT::COE_CREATE_HASDATA == eClassEvent)
        {
            m_pKernelModule->AddEventCallBack(self, NFED_ON_CLIENT_ENTER_SCENE, this, &NFCSceneProcessModule::OnEnterSceneEvent);
            m_pKernelModule->AddEventCallBack(self, NFED_ON_CLIENT_LEAVE_SCENE, this, &NFCSceneProcessModule::OnLeaveSceneEvent);
        }
    }

    return 0;
}

E_SCENE_TYPE NFCSceneProcessModule::GetCloneSceneType(const int nSceneID)
{
    char szSceneIDName[MAX_PATH] = { 0 };
    sprintf(szSceneIDName, "%d", nSceneID);
    if (m_pElementInfoModule->ExistElement(szSceneIDName))
    {
        return (E_SCENE_TYPE)m_pElementInfoModule->GetPropertyInt(szSceneIDName, NFrame::Scene::CanClone());
    }

    return SCENE_TYPE_ERROR;
}

bool NFCSceneProcessModule::IsCloneScene(const int nSceneID)
{
    return GetCloneSceneType(nSceneID) == 1;
}

bool NFCSceneProcessModule::ApplyCloneGroup(const int nSceneID, int& nGroupID)
{
    nGroupID = CreateCloneScene(nSceneID);

    return true;
}

bool NFCSceneProcessModule::ExitCloneGroup(const int nSceneID, const int& nGroupID)
{
    return m_pKernelModule->ExitGroupScene(nSceneID, nGroupID);
}

bool NFCSceneProcessModule::LoadSceneResource(const int nSceneID)
{
    char szSceneIDName[MAX_PATH] = { 0 };
    sprintf(szSceneIDName, "%d", nSceneID);

    const std::string& strSceneFilePath = m_pElementInfoModule->GetPropertyString(szSceneIDName, NFrame::Scene::FilePath());
    const int nCanClone = m_pElementInfoModule->GetPropertyInt(szSceneIDName, NFrame::Scene::CanClone());

    //������Ӧ��Դ
    NF_SHARE_PTR<NFMapEx<std::string, SceneSeedResource>> pSceneResourceMap = mtSceneResourceConfig.GetElement(nSceneID);
    if (!pSceneResourceMap.get())
    {
        pSceneResourceMap = NF_SHARE_PTR<NFMapEx<std::string, SceneSeedResource>>(NF_NEW NFMapEx<std::string, SceneSeedResource>());
        mtSceneResourceConfig.AddElement(nSceneID, pSceneResourceMap);
    }

    rapidxml::file<> xFileSource(strSceneFilePath.c_str());
    rapidxml::xml_document<>  xFileDoc;
    xFileDoc.parse<0>(xFileSource.data());

    //��Դ�ļ��б�
    rapidxml::xml_node<>* pSeedFileRoot = xFileDoc.first_node();
    for (rapidxml::xml_node<>* pSeedFileNode = pSeedFileRoot->first_node(); pSeedFileNode; pSeedFileNode = pSeedFileNode->next_sibling())
    {
        //���Ӿ�����Ϣ
        std::string strSeedID = pSeedFileNode->first_attribute("ID")->value();
        std::string strConfigID = pSeedFileNode->first_attribute("NPCConfigID")->value();
        float fSeedX = lexical_cast<float>(pSeedFileNode->first_attribute("SeedX")->value());
        float fSeedY = lexical_cast<float>(pSeedFileNode->first_attribute("SeedY")->value());
        float fSeedZ = lexical_cast<float>(pSeedFileNode->first_attribute("SeedZ")->value());

        if (!m_pElementInfoModule->ExistElement(strConfigID))
        {
            assert(0);
        }

        NF_SHARE_PTR<SceneSeedResource> pSeedResource = pSceneResourceMap->GetElement(strSeedID);
        if (!pSeedResource.get())
        {
            pSeedResource = NF_SHARE_PTR<SceneSeedResource>(NF_NEW SceneSeedResource());
            pSceneResourceMap->AddElement(strSeedID, pSeedResource);
        }

        pSeedResource->strSeedID = strSeedID;
        pSeedResource->strConfigID = strConfigID;
        pSeedResource->fSeedX = fSeedX;
        pSeedResource->fSeedY = fSeedY;
        pSeedResource->fSeedZ = fSeedZ;

    }

    return true;
}

void NFCSceneProcessModule::OnClienSwapSceneProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    //CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckSwapScene);
    //NFCDataList varEntry;
    //varEntry << pObject->Self();
    //varEntry << 0;
    //varEntry << xMsg.scene_id();
    //varEntry << -1;

    //const NFGUID self = NFINetModule::PBToNF((xMsg.selfid()));
}