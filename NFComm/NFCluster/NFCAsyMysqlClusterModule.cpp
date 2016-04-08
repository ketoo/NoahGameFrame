#include "NFCAsyMysqlClusterModule.h"
#include "NFCMysqlDriverManager.h"

bool SMysqlParam::PackParam(std::string& strData)
{
    try
    {
        NFMsg::PackMysqlParam xMsg;

        xMsg.set_strrecordname(strRecordName);
        xMsg.set_strkey(strKey);
        xMsg.set_bexit(bExit);
        xMsg.set_nreqid(nReqID);
        xMsg.set_nret(nRet);
        xMsg.set_etype(eType);

        for (int i = 0; i < fieldVec.size(); i++)
        {
            const std::string& strFiled = fieldVec[i];
            xMsg.add_fieldveclist(strFiled);
        }

        for (int i = 0; i < valueVec.size(); i++)
        {
            const std::string& strValue = valueVec[i];
            xMsg.add_valueveclist(strValue);
        }

        return xMsg.SerializeToString(&strData);
    }
    catch (...)
    {
        return false;
    }

    return true;
}


bool SMysqlParam::UnPackParam(const std::string& strData)
{
    try
    {
        NFMsg::PackMysqlParam xMsg;
        if (!xMsg.ParseFromString(strData))
        {
            return false;
        }

        strRecordName            = xMsg.strrecordname();
        strKey                   = xMsg.strkey();
        bExit                    = xMsg.bexit();
        nReqID                   = xMsg.nreqid();
        nRet                     = xMsg.nret();
        eType                    = (EMysqlOPRType)xMsg.etype();

        for (int i = 0; i < xMsg.fieldveclist_size(); i++)
        {
            const std::string& strField = xMsg.fieldveclist(i);
            fieldVec.push_back(strField);
        }

        for (int i = 0; i < xMsg.valueveclist_size(); i++)
        {
            const std::string& strValue = xMsg.valueveclist(i);
            valueVec.push_back(strValue);
        }

    }
    catch (...)
    {
        return false;
    }

    return true;
}

bool NFCMysqlComponent::Init()
{
    return true;
}

bool NFCMysqlComponent::AfterInit()
{
    m_pMysqlDriverManager = NF_SHARE_PTR<NFIMysqlDriverManager>(NF_NEW NFCMysqlDriverManager());

    return true;
}

int NFCMysqlComponent::OnASyncEvent(const NFGUID& self, const int event, std::string& arg)
{
    switch (event)
    {
        case NFCAsyMysqlClusterModule::ACOTERMYSQLEVENT_USEDB:
        {
            return OnASyUseMysqlEvent(self, event, arg);
        }
        break;
        case NFCAsyMysqlClusterModule::ACOTERMYSQLEVENT_INISERVER:
        {
            return OnASyAddMysqlServerEvent(self, event, arg);
        }
        break;
        case NFCAsyMysqlClusterModule::ACOTERMYSQLEVENT_KEEPALIVESERVER:
        {
            return OnASyKeepServerAliveEvent(self, event, arg);
        }
        break;
        default:
            break;
    }

    return 0;
}

int NFCMysqlComponent::OnASyUseMysqlEvent(const NFGUID& self, const int event, std::string& arg)
{
    SMysqlParam xparam;
    if (!xparam.UnPackParam(arg))
    {
        return -1;
    }

    if (!m_pMysqlDriverManager.get())
    {
        return -2;
    }

    NFIMysqlDriver* pDriver = m_pMysqlDriverManager->GetMysqlDriver();
    if (NULL == pDriver)
    {
        return -3;
    }

    switch (xparam.eType)
    {
        case SMysqlParam::EMYSQLOPRTYPE_UPDATA:
        {
            if (pDriver->Updata(xparam.strRecordName, xparam.strKey, xparam.fieldVec, xparam.valueVec))
            {
                xparam.nRet = -1;
            }
        }
        break;
        case SMysqlParam::EMYSQLOPRTYPE_QUERY :
        {
            if (pDriver->Query(xparam.strRecordName, xparam.strKey, xparam.fieldVec, xparam.valueVec))
            {
                xparam.nRet = -1;
            }
        }
        break;
        case SMysqlParam::EMYSQLOPRTYPE_SELECT:
        {
            if (pDriver->Select(xparam.strRecordName, xparam.strKey, xparam.fieldVec, xparam.valueVec))
            {
                xparam.nRet = -1;
            }
        }
        break;
        case SMysqlParam::EMYSQLOPRTYPE_DELETE:
        {
            if (pDriver->Delete(xparam.strRecordName, xparam.strKey))
            {
                xparam.nRet = -1;
            }
        }
        break;
        case SMysqlParam::EMYSQLOPRTYPE_EXISTS:
        {
            bool bExit = false;
            if (pDriver->Exists(xparam.strRecordName, xparam.strKey, bExit))
            {
                xparam.bExit = bExit;
                xparam.nRet = -1;
            }
        }
        break;
        case SMysqlParam::EMYSQLOPRTYPE_KEYS  :
        {
            if (pDriver->Keys(xparam.strRecordName, xparam.strKey, xparam.valueVec))
            {
                xparam.nRet = -1;
            }
        }
        break;
        default:
            break;
    }


    xparam.PackParam(arg);
    return 0;
}

int NFCMysqlComponent::OnASyAddMysqlServerEvent(const NFGUID& self, const int event, std::string& arg)
{
    NFMsg::PackMysqlServerInfo xMsg;
    if (!xMsg.ParseFromString(arg))
    {
        return -1;
    }

    if (!m_pMysqlDriverManager->AddMysqlServer(xMsg.nserverid(), "", xMsg.strdnsip(), xMsg.nport(), xMsg.strdbname(), xMsg.strdbuser(), xMsg.strdbpwd(), xMsg.nrconnecttime(), xMsg.nrconnecount()))
    {
        return -2;
    }

    return 0;
}

int NFCMysqlComponent::OnASyKeepServerAliveEvent(const NFGUID& self, const int event, std::string& arg)
{
    m_pMysqlDriverManager->CheckMysql();

    return 0;
}

NF_SHARE_PTR<NFIComponent> NFCMysqlComponent::CreateNewInstance()
{
    return  NF_SHARE_PTR<NFIComponent> (NF_NEW  NFCMysqlComponent(NFGUID(1, 2), ""));
}

NFCAsyMysqlClusterModule::NFCAsyMysqlClusterModule(NFIPluginManager* p)
{
    pPluginManager = p;
    mnLastCheckTime = 0;
}

NFCAsyMysqlClusterModule::~NFCAsyMysqlClusterModule()
{

}

bool NFCAsyMysqlClusterModule::Init()
{
    return true;
}

bool NFCAsyMysqlClusterModule::Shut()
{

    return true;
}

bool NFCAsyMysqlClusterModule::Execute()
{

    return true;
}

bool NFCAsyMysqlClusterModule::AfterInit()
{

    return true;
}

bool NFCAsyMysqlClusterModule::Updata(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlParam> pParam(NF_NEW SMysqlParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKey;
    pParam->fieldVec = fieldVec;
    pParam->valueVec = valueVec;
    pParam->nReqID = nCurReqID++;
    pParam->eType = SMysqlParam::EMYSQLOPRTYPE_UPDATA;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlClusterModule::Query(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnVeckKeyValueRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlParam> pParam(NF_NEW SMysqlParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKey;
    pParam->fieldVec = fieldVec;
    pParam->nReqID = nCurReqID++;
    pParam->eType = SMysqlParam::EMYSQLOPRTYPE_QUERY;
    pParam->mFunReturnVeckKeyValueRsp = mFunReturnVeckKeyValueRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlClusterModule::Select(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnVeckKeyValueRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlParam> pParam(NF_NEW SMysqlParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKey;
    pParam->fieldVec = fieldVec;
    pParam->nReqID = nCurReqID++;
    pParam->eType = SMysqlParam::EMYSQLOPRTYPE_SELECT;
    pParam->mFunReturnVeckKeyValueRsp = mFunReturnVeckKeyValueRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlClusterModule::Delete(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlParam> pParam(NF_NEW SMysqlParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKey;
    pParam->nReqID = nCurReqID++;
    pParam->eType = SMysqlParam::EMYSQLOPRTYPE_DELETE;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlClusterModule::Exists(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const MYSQL_RETURN_INT_FUNCTOR& mFunReturnIntRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlParam> pParam(NF_NEW SMysqlParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKey;
    pParam->nReqID = nCurReqID++;
    pParam->eType = SMysqlParam::EMYSQLOPRTYPE_EXISTS;
    pParam->mFunReturnIntRsp = mFunReturnIntRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlClusterModule::Keys(const NFGUID& self, const std::string& strRecordName, const std::string& strKeyName, const MYSQL_RETURN_VECVALUE_FUNCTOR& mFunReturnVecValueRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlParam> pParam(NF_NEW SMysqlParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKeyName;
    pParam->nReqID = nCurReqID++;
    pParam->eType = SMysqlParam::EMYSQLOPRTYPE_KEYS;
    pParam->mFunReturnVecValueRsp = mFunReturnVecValueRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlClusterModule::StartActorPool(const int nCount)
{
    NFIActorManager* pActorManager = pPluginManager->GetActorManager();
    if (NULL == pActorManager)
    {
        return false;
    }

    for (int i = 0; i < nCount; i++)
    {
        int nActorID = pActorManager->RequireActor<NFCMysqlComponent>(this, &NFCAsyMysqlClusterModule::RequestAsyEnd);
        if (nActorID > 0)
        {
            mActorList.AddElement(i, NF_SHARE_PTR<int> (NF_NEW int(nActorID)));
        }
    }

    return true;
}

bool NFCAsyMysqlClusterModule::CloseActorPool()
{
    int nActor = 0;
    NFIActorManager* pActorManager = pPluginManager->GetActorManager();
    if (NULL == pActorManager)
    {
        return false;
    }

    for (NF_SHARE_PTR<int> pData = mActorList.First(nActor); pData != NULL; pData = mActorList.Next(nActor))
    {
        pActorManager->ReleaseActor(nActor);
    }

    mActorList.ClearAll();
    return true;
}

int NFCAsyMysqlClusterModule::ApplyRequest(NF_SHARE_PTR<SMysqlParam> pParam)
{
    NFIActorManager* pActorManager = pPluginManager->GetActorManager();
    if (NULL == pActorManager)
    {
        return -1;
    }

    int nAcotrID = GetActor();
    if (nAcotrID <= 0)
    {
        return -2;
    }

    std::string arg;
    const int nEvetID = ACOTERMYSQLEVENT_USEDB;

    if (!pParam->PackParam(arg))
    {
        return -3;
    }

    if (!mReqList.AddElement(pParam->nReqID, pParam))
    {
        return -4;
    }

    if (!pActorManager->SendMsgToActor(nAcotrID, pParam->self, nEvetID, arg))
    {
        mReqList.RemoveElement(pParam->nReqID);
        return -5;
    }

    return 0;
}

int NFCAsyMysqlClusterModule::RequestAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    switch (nEventID)
    {
        case NFCAsyMysqlClusterModule::ACOTERMYSQLEVENT_USEDB:
        {
            return OnUseMysqlAsyEnd(self, nFormActor, nEventID, strData);
        }
        break;
        case NFCAsyMysqlClusterModule::ACOTERMYSQLEVENT_INISERVER:
        {
            return OnAddMysqlServerAsyEnd(self, nFormActor, nEventID, strData);
        }
        break;
        case NFCAsyMysqlClusterModule::ACOTERMYSQLEVENT_KEEPALIVESERVER:
        {
            return OnKeepServerAliveAsyEnd(self, nFormActor, nEventID, strData);
        }
        break;
        default:
            break;
    }

    return 0;
}

int NFCAsyMysqlClusterModule::GetActor()
{
    if (mActorList.Count() <= 0)
    {
        return -1;
    }

    mnSuitIndex ++;
    mnSuitIndex = mnSuitIndex % mActorList.Count();

    int i = 0;
    for (int* pData = mActorList.FirstNude(); pData != NULL ; pData = mActorList.NextNude())
    {
        if (i < mActorList.Count() && i == mnSuitIndex)
        {
            return *pData;
        }

        ++i;
    }

    return -1;
}

bool NFCAsyMysqlClusterModule::AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime /*= 10*/, const int nRconneCount /*= -1*/)
{
    NFIActorManager* pActorManager = pPluginManager->GetActorManager();
    if (NULL == pActorManager)
    {
        return false;
    }

    NFMsg::PackMysqlServerInfo xMsg;

    xMsg.set_nrconnecttime(nRconnectTime);
    xMsg.set_nrconnecount(nRconneCount);
    xMsg.set_nport(nPort);
    xMsg.set_strdbname(strDBName);
    xMsg.set_strdnsip(strIP);
    xMsg.set_strdbuser(strDBUser);
    xMsg.set_strdbpwd(strDBPwd);
    xMsg.set_nserverid(nServerID);
    std::string arg;

    if (!xMsg.SerializeToString(&arg))
    {
        return false;
    }

    const int nEvetID = ACOTERMYSQLEVENT_INISERVER;
    for (int* pData = mActorList.FirstNude(); pData != NULL ; pData = mActorList.NextNude())
    {
        int nAcotrID = *pData;
        pActorManager->SendMsgToActor(nAcotrID, NFGUID(), nEvetID, arg);
    }

    return true;
}


bool NFCAsyMysqlClusterModule::KeepAliveMysqlServer()
{
    NFIActorManager* pActorManager = pPluginManager->GetActorManager();
    if (NULL == pActorManager)
    {
        return false;
    }

    std::string arg;
    const int nEvetID = ACOTERMYSQLEVENT_KEEPALIVESERVER;
    for (int* pData = mActorList.FirstNude(); pData != NULL ; pData = mActorList.NextNude())
    {
        int nAcotrID = *pData;
        pActorManager->SendMsgToActor(nAcotrID, NFGUID(), nEvetID, arg);
    }

    return true;
}

int NFCAsyMysqlClusterModule::OnUseMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    SMysqlParam xResultparam;
    if (!xResultparam.UnPackParam(strData))
    {
        return -1;
    }

    NF_SHARE_PTR<SMysqlParam> pReqData = mReqList.GetElement(xResultparam.nReqID);
    if (NULL == pReqData)
    {
        return -2;
    }

    switch (pReqData->eType)
    {
        case SMysqlParam::EMYSQLOPRTYPE_DELETE:
        case SMysqlParam::EMYSQLOPRTYPE_UPDATA:
        {
            if (pReqData->mFunReturnRsp)
            {
                pReqData->mFunReturnRsp(pReqData->self, pReqData->nRet, pReqData->mstrUseData);
            }
        }
        break;
        case SMysqlParam::EMYSQLOPRTYPE_SELECT:
        case SMysqlParam::EMYSQLOPRTYPE_QUERY :
        {
            if (pReqData->mFunReturnVeckKeyValueRsp)
            {
                pReqData->mFunReturnVeckKeyValueRsp(pReqData->self, pReqData->nRet, pReqData->fieldVec, pReqData->valueVec, pReqData->mstrUseData);
            }
        }
        break;
        case SMysqlParam::EMYSQLOPRTYPE_EXISTS:
        {
            if (pReqData->mFunReturnIntRsp)
            {
                pReqData->mFunReturnIntRsp(pReqData->self, pReqData->nRet, pReqData->bExit, pReqData->mstrUseData);
            }
        }
        break;
        case SMysqlParam::EMYSQLOPRTYPE_KEYS  :
        {
            if (pReqData->mFunReturnVecValueRsp)
            {
                pReqData->mFunReturnVecValueRsp(pReqData->self, pReqData->nRet, pReqData->valueVec, pReqData->mstrUseData);
            }
        }
        break;
        default:
            break;
    }

    mReqList.RemoveElement(xResultparam.nReqID);
    return 0;
}

int NFCAsyMysqlClusterModule::OnAddMysqlServerAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    return 0;
}

int NFCAsyMysqlClusterModule::OnKeepServerAliveAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    return 0;
}
