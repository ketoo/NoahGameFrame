#include "NFCAsyMysqlModule.h"
#include "NFCMysqlDriverManager.h"

bool SMysqlBaseParam::PackParam(std::string& strData)
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


bool SMysqlBaseParam::UnPackParam(const std::string& strData)
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
        eType                    = (NFIAsyMysqlModule::ACOTERMYSQLEVENT)xMsg.etype();

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
    int nRet = 0;
    switch (event)
    {
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_UPDATA:
        {
            nRet = OnASyUpdataMysqlEvent(self, event, arg);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_QUERY:
        {
            nRet = OnASyQueryMysqlEvent(self, event, arg);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_SELECT:
        {
            nRet = OnASySelectMysqlEvent(self, event, arg);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_DELETE:
        {
            nRet = OnASyDeleteMysqlEvent(self, event, arg);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_EXISTS:
        {
            nRet = OnASyExistsMysqlEvent(self, event, arg);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_KEYS:
        {
            nRet = OnASyKeysMysqlEvent(self, event, arg);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_INISERVER:
        {
            nRet = OnASyAddMysqlServerEvent(self, event, arg);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_KEEPALIVESERVER:
        {
            nRet = OnASyKeepServerAliveEvent(self, event, arg);
        }
        break;
        default:
            break;
    }

    return nRet;
}

int NFCMysqlComponent::OnASyUpdataMysqlEvent(const NFGUID& self, const int event, std::string& arg)
{
    SMysqlBaseParam xparam;
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

    if (xparam.eType != NFCAsyMysqlModule::ACOTERMYSQLEVENT_UPDATA)
    {
        return -4;
    }

    if (!pDriver->Updata(xparam.strRecordName, xparam.strKey, xparam.fieldVec, xparam.valueVec))
    {
        xparam.nRet = -1;
    }

    xparam.PackParam(arg);
    return 0;
}


int NFCMysqlComponent::OnASyQueryMysqlEvent(const NFGUID& self, const int event, std::string& arg)
{
    SMysqlBaseParam xparam;
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

    if (xparam.eType != NFCAsyMysqlModule::ACOTERMYSQLEVENT_QUERY)
    {
        return -4;
    }

    if (!pDriver->Query(xparam.strRecordName, xparam.strKey, xparam.fieldVec, xparam.valueVec))
    {
        xparam.nRet = -1;
    }

    xparam.PackParam(arg);
    return 0;
}


int NFCMysqlComponent::OnASySelectMysqlEvent(const NFGUID& self, const int event, std::string& arg)
{
    SMysqlBaseParam xparam;
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

    if (xparam.eType != NFCAsyMysqlModule::ACOTERMYSQLEVENT_SELECT)
    {
        return -4;
    }

    if (!pDriver->Select(xparam.strRecordName, xparam.strKey, xparam.fieldVec, xparam.valueVec))
    {
        xparam.nRet = -1;
    }

    xparam.PackParam(arg);
    return 0;
}


int NFCMysqlComponent::OnASyDeleteMysqlEvent(const NFGUID& self, const int event, std::string& arg)
{
    SMysqlBaseParam xparam;
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

    if (xparam.eType != NFCAsyMysqlModule::ACOTERMYSQLEVENT_DELETE)
    {
        return -4;
    }

    if (!pDriver->Delete(xparam.strRecordName, xparam.strKey))
    {
        xparam.nRet = -1;
    }

    xparam.PackParam(arg);
    return 0;
}


int NFCMysqlComponent::OnASyExistsMysqlEvent(const NFGUID& self, const int event, std::string& arg)
{
    SMysqlBaseParam xparam;
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

    if (xparam.eType != NFCAsyMysqlModule::ACOTERMYSQLEVENT_EXISTS)
    {
        return -4;
    }

    bool bExit = false;
    if (!pDriver->Exists(xparam.strRecordName, xparam.strKey, bExit))
    {
        xparam.bExit = bExit;
        xparam.nRet = -1;
    }

    xparam.PackParam(arg);
    return 0;
}


int NFCMysqlComponent::OnASyKeysMysqlEvent(const NFGUID& self, const int event, std::string& arg)
{
    SMysqlBaseParam xparam;
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

    if (xparam.eType != NFCAsyMysqlModule::ACOTERMYSQLEVENT_KEYS)
    {
        return -4;
    }

    if (!pDriver->Keys(xparam.strRecordName, xparam.strKey, xparam.valueVec))
    {
        xparam.nRet = -1;
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

NFCAsyMysqlModule::NFCAsyMysqlModule(NFIPluginManager* p)
{
    pPluginManager = p;
    mnLastCheckTime = 0;
}

NFCAsyMysqlModule::~NFCAsyMysqlModule()
{

}

bool NFCAsyMysqlModule::Init()
{
    return true;
}

bool NFCAsyMysqlModule::Shut()
{

    return true;
}

bool NFCAsyMysqlModule::Execute()
{

    return true;
}

bool NFCAsyMysqlModule::AfterInit()
{
	m_pActorModule = pPluginManager->FindModule<NFIActorModule>();

	StartActorPool(10);

    return true;
}

bool NFCAsyMysqlModule::Updata(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const std::vector<std::string>& valueVec, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlUpDataParam> pParam(NF_NEW SMysqlUpDataParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKey;
    pParam->fieldVec = fieldVec;
    pParam->valueVec = valueVec;
    pParam->nReqID = nCurReqID++;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->eType) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlModule::Query(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnVeckKeyValueRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlQueryParam> pParam(NF_NEW SMysqlQueryParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKey;
    pParam->fieldVec = fieldVec;
    pParam->nReqID = nCurReqID++;
    pParam->mFunReturnVeckKeyValueRsp = mFunReturnVeckKeyValueRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->eType) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlModule::Select(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const std::vector<std::string>& fieldVec, const MYSQL_RETURN_VECKEY_VECVALUE_FUNCTOR& mFunReturnVeckKeyValueRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlSelectParam> pParam(NF_NEW SMysqlSelectParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKey;
    pParam->fieldVec = fieldVec;
    pParam->nReqID = nCurReqID++;
    pParam->mFunReturnVeckKeyValueRsp = mFunReturnVeckKeyValueRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->eType) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlModule::Delete(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const MYSQL_RETURN_FUNCTOR& mFunReturnRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlDeleteParam> pParam(NF_NEW SMysqlDeleteParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKey;
    pParam->nReqID = nCurReqID++;
    pParam->mFunReturnRsp = mFunReturnRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->eType) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlModule::Exists(const NFGUID& self, const std::string& strRecordName, const std::string& strKey, const MYSQL_RETURN_INT_FUNCTOR& mFunReturnIntRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlExistsParam> pParam(NF_NEW SMysqlExistsParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKey;
    pParam->nReqID = nCurReqID++;
    pParam->mFunReturnIntRsp = mFunReturnIntRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->eType) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlModule::Keys(const NFGUID& self, const std::string& strRecordName, const std::string& strKeyName, const MYSQL_RETURN_VECVALUE_FUNCTOR& mFunReturnVecValueRsp, const std::string& strUseData)
{
    NF_SHARE_PTR<SMysqlKeysParam> pParam(NF_NEW SMysqlKeysParam());
    if (NULL == pParam)
    {
        return false;
    }

    pParam->strRecordName = strRecordName;
    pParam->strKey = strKeyName;
    pParam->nReqID = nCurReqID++;
    pParam->mFunReturnVecValueRsp = mFunReturnVecValueRsp;
    pParam->mstrUseData = strUseData;
    pParam->self = self;

    if (ApplyRequest(pParam, pParam->eType) < 0)
    {
        return false;
    }

    return true;
}

bool NFCAsyMysqlModule::StartActorPool(const int nCount)
{
    for (int i = 0; i < nCount; i++)
    {
		int nActorID = m_pActorModule->RequireActor<NFCMysqlComponent>(this, &NFCAsyMysqlModule::RequestAsyEnd);
        if (nActorID > 0)
        {
            mActorList.AddElement(i, NF_SHARE_PTR<int> (NF_NEW int(nActorID)));
        }
    }

    return true;
}

bool NFCAsyMysqlModule::CloseActorPool()
{
    int nActor = 0;
    for (NF_SHARE_PTR<int> pData = mActorList.First(nActor); pData != NULL; pData = mActorList.Next(nActor))
    {
		m_pActorModule->ReleaseActor(nActor);
    }

    mActorList.ClearAll();
    return true;
}

int NFCAsyMysqlModule::ApplyRequest(NF_SHARE_PTR<SMysqlBaseParam> pParam, const int nEvetID/* = ACOTERMYSQLEVENT_USEDB*/)
{
    int nAcotrID = GetActor();
    if (nAcotrID <= 0)
    {
        return -2;
    }

    std::string arg;
    if (!pParam->PackParam(arg))
    {
        return -3;
    }

    if (!mReqList.AddElement(pParam->nReqID, pParam))
    {
        return -4;
    }

    if (!m_pActorModule->SendMsgToActor(nAcotrID, pParam->self, nEvetID, arg))
    {
        mReqList.RemoveElement(pParam->nReqID);
        return -5;
    }

    return 0;
}

int NFCAsyMysqlModule::RequestAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    int nRet = 0;
    switch (nEventID)
    {
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_UPDATA:
        {
            nRet = OnUpDataMysqlAsyEnd(self, nFormActor, nEventID, strData);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_QUERY:
        {
            nRet = OnQueryMysqlAsyEnd(self, nFormActor, nEventID, strData);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_SELECT:
        {
            nRet = OnSelectMysqlAsyEnd(self, nFormActor, nEventID, strData);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_DELETE:
        {
            nRet = OnDeleteMysqlAsyEnd(self, nFormActor, nEventID, strData);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_EXISTS:
        {
            nRet = OnExistsMysqlAsyEnd(self, nFormActor, nEventID, strData);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_KEYS:
        {
            nRet = OnUpKeyMysqlAsyEnd(self, nFormActor, nEventID, strData);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_INISERVER:
        {
            nRet = OnAddMysqlServerAsyEnd(self, nFormActor, nEventID, strData);
        }
        break;
        case NFCAsyMysqlModule::ACOTERMYSQLEVENT_KEEPALIVESERVER:
        {
            nRet = OnKeepServerAliveAsyEnd(self, nFormActor, nEventID, strData);
        }
        break;
        default:
            break;
    }

    return nRet;
}

int NFCAsyMysqlModule::GetActor()
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

bool NFCAsyMysqlModule::AddMysqlServer(const int nServerID, const std::string& strDns, const std::string& strIP, const int nPort, const std::string strDBName, const std::string strDBUser, const std::string strDBPwd, const int nRconnectTime /*= 10*/, const int nRconneCount /*= -1*/)
{
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
		m_pActorModule->SendMsgToActor(nAcotrID, NFGUID(), nEvetID, arg);
    }

    return true;
}


bool NFCAsyMysqlModule::KeepAliveMysqlServer()
{
    std::string arg;
    const int nEvetID = ACOTERMYSQLEVENT_KEEPALIVESERVER;
    for (int* pData = mActorList.FirstNude(); pData != NULL ; pData = mActorList.NextNude())
    {
        int nAcotrID = *pData;
		m_pActorModule->SendMsgToActor(nAcotrID, NFGUID(), nEvetID, arg);
    }

    return true;
}

int NFCAsyMysqlModule::OnUpDataMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    SMysqlUpDataParam* pReqData = UnpackResult<SMysqlUpDataParam>(strData);
    if (NULL == pReqData)
    {
        return -3;
    }

    if (pReqData->mFunReturnRsp)
    {
        pReqData->mFunReturnRsp(pReqData->self, pReqData->nRet, pReqData->mstrUseData);
    }

    mReqList.RemoveElement(pReqData->nReqID);
    return 0;
}


int NFCAsyMysqlModule::OnQueryMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    SMysqlQueryParam* pReqData = UnpackResult<SMysqlQueryParam>(strData);
    if (NULL == pReqData)
    {
        return -3;
    }

    if (pReqData->mFunReturnVeckKeyValueRsp)
    {
        pReqData->mFunReturnVeckKeyValueRsp(pReqData->self, pReqData->nRet, pReqData->fieldVec, pReqData->valueVec, pReqData->mstrUseData);
    }

    mReqList.RemoveElement(pReqData->nReqID);
    return 0;
}


int NFCAsyMysqlModule::OnSelectMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    SMysqlSelectParam* pReqData = UnpackResult<SMysqlSelectParam>(strData);
    if (NULL == pReqData)
    {
        return -3;
    }

    if (pReqData->mFunReturnVeckKeyValueRsp)
    {
        pReqData->mFunReturnVeckKeyValueRsp(pReqData->self, pReqData->nRet, pReqData->fieldVec, pReqData->valueVec, pReqData->mstrUseData);
    }

    mReqList.RemoveElement(pReqData->nReqID);
    return 0;
}


int NFCAsyMysqlModule::OnDeleteMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    SMysqlDeleteParam* pReqData = UnpackResult<SMysqlDeleteParam>(strData);
    if (NULL == pReqData)
    {
        return -3;
    }

    if (pReqData->mFunReturnRsp)
    {
        pReqData->mFunReturnRsp(pReqData->self, pReqData->nRet, pReqData->mstrUseData);
    }

    mReqList.RemoveElement(pReqData->nReqID);
    return 0;
}


int NFCAsyMysqlModule::OnExistsMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    SMysqlExistsParam* pReqData = UnpackResult<SMysqlExistsParam>(strData);
    if (NULL == pReqData)
    {
        return -3;
    }

    if (pReqData->mFunReturnIntRsp)
    {
        pReqData->mFunReturnIntRsp(pReqData->self, pReqData->nRet, pReqData->bExit, pReqData->mstrUseData);
    }

    mReqList.RemoveElement(pReqData->nReqID);
    return 0;
}


int NFCAsyMysqlModule::OnUpKeyMysqlAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    SMysqlKeysParam* pReqData = UnpackResult<SMysqlKeysParam>(strData);
    if (NULL == pReqData)
    {
        return -3;
    }

    if (pReqData->mFunReturnVecValueRsp)
    {
        pReqData->mFunReturnVecValueRsp(pReqData->self, pReqData->nRet, pReqData->valueVec, pReqData->mstrUseData);
    }

    mReqList.RemoveElement(pReqData->nReqID);
    return 0;
}

int NFCAsyMysqlModule::OnAddMysqlServerAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    return 0;
}

int NFCAsyMysqlModule::OnKeepServerAliveAsyEnd(const NFGUID& self, const int nFormActor, const int nEventID, const std::string& strData)
{
    return 0;
}

template<typename ClassParam>
ClassParam* NFCAsyMysqlModule::UnpackResult(const std::string& strMsgData)
{
    SMysqlBaseParam xResultparam;
    if (!xResultparam.UnPackParam(strMsgData))
    {
        return NULL;
    }

    NF_SHARE_PTR<SMysqlBaseParam> pReqBaseData = mReqList.GetElement(xResultparam.nReqID);
    if (NULL == pReqBaseData)
    {
        return NULL;
    }

    ClassParam* pReqData = dynamic_cast<ClassParam*>(pReqBaseData.get());
    if (NULL == pReqData)
    {
        return NULL;
    }

    pReqData->valueVec = xResultparam.valueVec;
    pReqData->bExit = xResultparam.bExit;
    pReqData->nRet = xResultparam.nRet;

    return pReqData;
}
