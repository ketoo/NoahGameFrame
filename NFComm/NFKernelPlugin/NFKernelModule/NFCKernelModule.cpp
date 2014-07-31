// -------------------------------------------------------------------------
//    @FileName      :    NFCKernelModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCKernelModule
//    @Desc             :
// -------------------------------------------------------------------------

#include <time.h>
#include <boost/bind.hpp>
#include "NFStackWalker.h"
#include "NFCKernelModule.h"
#include "NFComm/NFCore/NFIdentID.h"
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFCore/NFCValueList.h"
#include "NFComm/NFCore/NFCRecord.h"
#include "../NFKernelPlugin.h"

NFCKernelModule::NFCKernelModule(NFIPluginManager* p)
{
    fLastTotal = 0.0f;
    pPluginManager = p;
    
    m_pContainerModule = NF_NEW NFCContainerModule();

    InitRandom();
}

NFCKernelModule::~NFCKernelModule()
{
    NFCContainerInfo* pContainerInfo = m_pContainerModule->First();
    while (pContainerInfo)
    {
        NFCContainerGroupInfo* pContainerGroupInfo = pContainerInfo->First();
        while (pContainerGroupInfo)
        {
            delete pContainerGroupInfo;
            pContainerGroupInfo = NULL;
            pContainerGroupInfo = pContainerInfo->Next();
        }

        NFIDENTID ident = pContainerInfo->GetObjectSelf();
        NFIObject* pSceneObject = RemoveElement(ident);
        if (pSceneObject)
        {
            delete pSceneObject;
            pSceneObject = NULL;
        }

        pContainerInfo->ClearAll();
        delete pContainerInfo;
        pContainerInfo = NULL;

        pContainerInfo = m_pContainerModule->Next();
    }

    m_pContainerModule->ClearAll();
    delete m_pContainerModule;
    m_pContainerModule = NULL;
}

void NFCKernelModule::InitRandom()
{
    mvRandom.clear();

    int nRandomMax = 100000;
    mnRandomPos = 0;
    boost::uniform_real<> distribution(0, 1.0f) ;
    boost::mt19937 engine ;
    boost::variate_generator<boost::mt19937, boost::uniform_real<> > myrandom(engine, distribution);

    for (int i = 0; i < nRandomMax; i++)
    {
        mvRandom.push_back(myrandom());
    }
}

bool NFCKernelModule::Init()
{
    mtDeleteSelfList.clear();
    //mLogFile.open( "./Log/NFKernel.log" );

    // every process has a mnIdentIndex, and the ident maybe conflict
    // so...
    //mnIdentIndex

    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));

    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogModule);

    return true;
}

bool NFCKernelModule::Shut()
{
    //   mLogFile.close();

    return true;
}

bool NFCKernelModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    mnCurExeObject = 0;

    if (mtDeleteSelfList.size() > 0)
    {
        std::list<NFIDENTID>::iterator it = mtDeleteSelfList.begin();
        for (it; it != mtDeleteSelfList.end(); it++)
        {
            DestroyObject(*it);
        }
        mtDeleteSelfList.clear();
    }


    if (fLastTotal < 0.1f)
    {
        fLastTotal += fLasFrametime;
        return false;
    }

    //所有场景
    NFCContainerInfo* pContainerInfo = m_pContainerModule->First();
    while (pContainerInfo)
    {
        pContainerInfo->Execute(fLastTotal, fStartedTime);

        pContainerInfo = m_pContainerModule->Next();
    }

    NFIObject* pObject = First();
    while (pObject)
    {
        mnCurExeObject = pObject->Self();
        pObject->Execute(fLastTotal, fStartedTime);
        mnCurExeObject = 0;

        pObject = Next();
    }

    fLastTotal = 0.0f;

    return true;
}

bool NFCKernelModule::SetComponentEnable( const NFIDENTID& self, const std::string& strComponentName, const bool bEnable )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetComponentManager()->SetEnable(strComponentName, bEnable);
    }

    return false;
}

bool NFCKernelModule::QueryComponentEnable( const NFIDENTID& self, const std::string& strComponentName )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetComponentManager()->Enable(strComponentName);
    }

    return false;
}

bool NFCKernelModule::FindHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->FindHeartBeat(strHeartBeatName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);
    return false;
}

bool NFCKernelModule::RemoveHeartBeat(const NFIDENTID& self, const std::string& strHeartBeatName)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->RemoveHeartBeat(strHeartBeatName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return false;
}

int NFCKernelModule::Type(const NFIDENTID& self)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetPropertyInt("Type");
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return -1;
}

NFIObject* NFCKernelModule::CreateObject(const NFIDENTID& self, const int nContainerID, const int nGroupID, const std::string& strClassName, const std::string& strConfigIndex, const NFIValueList& arg)
{
    //int64_t nTimeBegin = GetTickCount64();

    NFIObject* pObject = NULL;
    NFIDENTID ident = self;

    //默认为1分组，0则是所有分组都看得见,-1则是容器
    if (ident.IsNull())
    {
        ident = NFIDENTID::CreateGUID();
    }

    if (GetElement(ident))
    {
        m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, ident, "The object has Exists", __FUNCTION__, __LINE__);
        return NULL;
    }

    NFCContainerInfo* pContainerInfo = m_pContainerModule->GetElement(nContainerID);
    if (!pContainerInfo)
    {
        m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, nContainerID, "There is no object", __FUNCTION__, __LINE__);

        return pObject;
    }

    NFIPropertyManager* pStaticClassPropertyManager = m_pLogicClassModule->GetClassPropertyManager(strClassName);
    NFIRecordManager* pStaticClassRecordManager = m_pLogicClassModule->GetClassRecordManager(strClassName);
    NFIComponentManager* pStaticClasComponentManager = m_pLogicClassModule->GetClassComponentManager(strClassName);
    if (pStaticClassPropertyManager && pStaticClassRecordManager && pStaticClasComponentManager)
    {

        pObject = NF_NEW NFCObject(ident, pPluginManager);
        //是否是应该晚点等到事件2时才加入容器，这样能保证进入容器的对象都是有完整数据的，否则因为协程的原因，其他对象找到他时他却没数据或者部分数据
        AddElement(ident, pObject);

        NFIPropertyManager* pPropertyManager = pObject->GetPropertyManager();
        NFIRecordManager* pRecordManager = pObject->GetRecordManager();
        NFIComponentManager* pComponentManager = pObject->GetComponentManager();

        //默认属性
        NFIProperty* pStaticConfigPropertyInfo = pStaticClassPropertyManager->First();
        while (pStaticConfigPropertyInfo)
        {
            pPropertyManager->AddProperty(ident,
                pStaticConfigPropertyInfo->GetKey(),
                pStaticConfigPropertyInfo->GetType(),
                pStaticConfigPropertyInfo->GetPublic(),
                pStaticConfigPropertyInfo->GetPrivate(),
                pStaticConfigPropertyInfo->GetSave(),
                pStaticConfigPropertyInfo->GetIndex(),
                pStaticConfigPropertyInfo->GetRelationValue());

            //通用回调，方便NET同步
            PROPERTY_EVENT_FUNCTOR functor = boost::bind(&NFCKernelModule::OnPropertyCommonEvent, this, _1, _2, _3, _4, _5);
            PROPERTY_EVENT_FUNCTOR_PTR functorPtr(NF_NEW PROPERTY_EVENT_FUNCTOR(functor));
            pObject->AddPropertyCallBack(pStaticConfigPropertyInfo->GetKey(), functorPtr);

            pStaticConfigPropertyInfo = pStaticClassPropertyManager->Next();
        }

        NFIRecord* pConfigRecordInfo = pStaticClassRecordManager->First();
        while (pConfigRecordInfo)
        {
            pRecordManager->AddRecord(ident,
                pConfigRecordInfo->GetName(),
                pConfigRecordInfo->GetInitData(),
                pConfigRecordInfo->GetKeyState(),
                pConfigRecordInfo->GetInitDesc(),
                pConfigRecordInfo->GetTag(),
                pConfigRecordInfo->GetRelatedRecord(),
                pConfigRecordInfo->GetRows(),
                pConfigRecordInfo->GetPublic(),
                pConfigRecordInfo->GetPrivate(),
                pConfigRecordInfo->GetSave(),
                pConfigRecordInfo->GetIndex());

            //通用回调，方便NET同步
            RECORD_EVENT_FUNCTOR functor = boost::bind(&NFCKernelModule::OnRecordCommonEvent, this, _1, _2, _3, _4, _5, _6, _7, _8);
            RECORD_EVENT_FUNCTOR_PTR functorPtr(NF_NEW RECORD_EVENT_FUNCTOR(functor));
            pObject->AddRecordCallBack(pConfigRecordInfo->GetName(), functorPtr);

            pConfigRecordInfo = pStaticClassRecordManager->Next();
        }

        NFIComponent* pConfigComponentInfo = pStaticClasComponentManager->First();
        while (pConfigComponentInfo)
        {
            pComponentManager->AddComponent(pConfigComponentInfo->ComponentName(), pConfigComponentInfo->LanguageName());

            pConfigComponentInfo = pStaticClasComponentManager->Next();
        }
        //////////////////////////////////////////////////////////////////////////
        //配置属性
        NFIPropertyManager* pConfigPropertyManager = m_pElementInfoModule->GetPropertyManager(strConfigIndex);
        NFIRecordManager* pConfigRecordManager = m_pElementInfoModule->GetRecordManager(strConfigIndex);

        if (pConfigPropertyManager && pConfigRecordManager)
        {
            NFIProperty* pConfigPropertyInfo = pConfigPropertyManager->First();
            while (pConfigPropertyInfo)
            {

                pPropertyManager->SetProperty(pConfigPropertyInfo);

                pConfigPropertyInfo = pConfigPropertyManager->Next();
            }
        }

        m_pEventProcessModule->DoEvent(ident, strClassName, CLASS_OBJECT_EVENT::COE_CREATE_NODATA, arg);

        //传入的属性赋值
        for (int i = 0; i < arg.GetCount() - 1; i += 2)
        {
            const std::string& strPropertyName = arg.StringVal(i);
            if ("ConfigID" != strPropertyName
                && "ClassName" != strPropertyName
                && "SceneID" != strPropertyName
                && "GroupID" != strPropertyName)
            {
                NFIProperty* pArgProperty = pStaticClassPropertyManager->GetElement(strPropertyName);
                if (pArgProperty)
                {
                    switch (pArgProperty->GetType())
                    {
                        case TDATA_INT:
                            pObject->SetPropertyInt(strPropertyName, arg.IntVal(i + 1));
                            break;
                        case TDATA_FLOAT:
                            pObject->SetPropertyFloat(strPropertyName, arg.FloatVal(i + 1));
                            break;
                        case TDATA_DOUBLE:
                            pObject->SetPropertyDouble(strPropertyName, arg.DoubleVal(i + 1));
                            break;
                        case TDATA_STRING:
                            pObject->SetPropertyString(strPropertyName, arg.StringVal(i + 1));
                            break;
                        case TDATA_OBJECT:
                            pObject->SetPropertyObject(strPropertyName, arg.ObjectVal(i + 1));
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        //放进容器//先进入场景，再进入层
        pObject->SetPropertyString("ConfigID", strConfigIndex);
        pObject->SetPropertyString("ClassName", strClassName);
        pObject->SetPropertyInt("SceneID", nContainerID);

        if (pContainerInfo->GetElement(nGroupID))
        {
            pContainerInfo->AddObjectToGroup(nGroupID, ident);
            pObject->SetPropertyInt("GroupID", nGroupID);
        }

        if (strClassName == "Player"
            ||  strClassName == "PublicPlayer"
            || strClassName == "Scene")
        {
            //int64_t nTimeBegin1 = GetTickCount64();

            m_pEventProcessModule->DoEvent(ident, strClassName, CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA, arg);

            //int64_t nTimeDis1 = GetTickCount64() - nTimeBegin1;
            //std::ostringstream str1;
            //str1 << "COE_CREATE_LOADDATA : ";
            //str1 << nTimeDis1;
            //m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, str1, __FUNCTION__, __LINE__);
        }

        //int64_t nTimeBegin2 = GetTickCount64();

        m_pEventProcessModule->DoEvent(ident, strClassName, CLASS_OBJECT_EVENT::COE_CREATE_BEFORE_EFFECT, arg);

        //int64_t nTimeDis2 = GetTickCount64() - nTimeBegin2;
        //std::ostringstream str2;
        //str2 << "COE_CREATE_BEFORE_EFFECT : ";
        //str2 << nTimeDis2;
        //m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, str2, __FUNCTION__, __LINE__);

        //int64_t nTimeBegin3 = GetTickCount64();

        m_pEventProcessModule->DoEvent(ident, strClassName, CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA, arg);

        m_pEventProcessModule->DoEvent(ident, strClassName, CLASS_OBJECT_EVENT::COE_CREATE_AFTER_EFFECT, arg);
        
        //int64_t nTimeDis3 = GetTickCount64() - nTimeBegin3;
        //std::ostringstream str3;
        //str3 << "COE_CREATE_EFFECTDATA : ";
        //str3 << nTimeDis3;
        //m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, str3, __FUNCTION__, __LINE__);

        //int64_t nTimeBegin4 = GetTickCount64();

        m_pEventProcessModule->DoEvent(ident, strClassName, CLASS_OBJECT_EVENT::COE_CREATE_HASDATA, arg);

        //int64_t nTimeDis4 = GetTickCount64() - nTimeBegin4;
        //std::ostringstream str4;
        //str4 << "COE_CREATE_HASDATA : ";
        //str4 << nTimeDis4;
        //m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, str4, __FUNCTION__, __LINE__);
    }

    //////////////////////////////////////////////////////////////////////////
    if (nGroupID < 0)
    {
        //容器
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "Create scene successed", nContainerID);
    }
    //else
    //{
    //    int64_t nTimeDis = GetTickCount64() - nTimeBegin;
    //    static int64_t totalTime = 0;
    //    totalTime += nTimeDis;
    //    std::ostringstream str;
    //    str << nTimeDis;
    //    str << " | ";
    //    str << totalTime;
    //    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, str, __FUNCTION__, __LINE__);
    //}

    return pObject;
}

bool NFCKernelModule::DestroyObject(const NFIDENTID& self)
{
    if (self == mnCurExeObject
        && !self.IsNull())
    {
        //自己的循环过程中，不能删除自己，得等下一帧才行
        return DestroySelf(self);
    }

    //需要同时从容器中删掉
    int nGroupID = GetPropertyInt(self, "GroupID");
    int nContainerID = GetPropertyInt(self, "SceneID");

    NFCContainerInfo* pContainerInfo = m_pContainerModule->GetElement(nContainerID);
    if (pContainerInfo)
    {
        //////////////////////////////////////////////////////////////////////////
        //判断自己是否是场景容器
        const std::string& strClassName = GetPropertyString(self, "ClassName");
        if (strClassName == std::string("Scene"))
        {
            return true;
        }
        else
        {
            if (pContainerInfo->RemoveObjectFromGroup(nGroupID, self))
            {
                

                m_pEventProcessModule->DoEvent(self, strClassName, CLASS_OBJECT_EVENT::COE_BEFOREDESTROY, NFCValueList());
                m_pEventProcessModule->DoEvent(self, strClassName, CLASS_OBJECT_EVENT::COE_DESTROY, NFCValueList());


                NFIObject* pObject = RemoveElement(self);
                if (pObject)
                {
                    //m_pLogModule->Log(NFILogModule::NLL_INFO_NORMAL, "DestroyObject=%lld, ClassName=%s, SceneID=%d, GroupID=%d \n", self.nData64, strClassName.c_str(), nContainerID, nGroupID);
                    //m_pLogModule->LogObject(NFILogModule::NLL_INFO_NORMAL, self, "Destroy object successed", __FUNCTION__, __LINE__);
                    pObject->Shut();
                    delete pObject;
                    pObject = NULL;

                    m_pEventProcessModule->RemoveEvent(self);
                }

                return true;
            }
        }

    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::FindProperty(const NFIDENTID& self, const std::string& strPropertyName)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->FindProperty(strPropertyName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName, const int nValue)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->SetPropertyInt(strPropertyName, nValue);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);
    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName, __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName,  const float fValue)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->SetPropertyFloat(strPropertyName, fValue);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetPropertyDouble(const NFIDENTID& self, const std::string& strPropertyName, const double dValue)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->SetPropertyDouble(strPropertyName, dValue);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetPropertyString(const NFIDENTID& self, const std::string& strPropertyName, const std::string& strValue)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->SetPropertyString(strPropertyName, strValue);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName, const NFIDENTID& objectValue)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->SetPropertyObject(strPropertyName, objectValue);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

int NFCKernelModule::GetPropertyInt(const NFIDENTID& self, const std::string& strPropertyName)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetPropertyInt(strPropertyName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return 0;
}

float NFCKernelModule::GetPropertyFloat(const NFIDENTID& self, const std::string& strPropertyName)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetPropertyFloat(strPropertyName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return 0.0f;
}

double NFCKernelModule::GetPropertyDouble(const NFIDENTID& self, const std::string& strPropertyName)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetPropertyDouble(strPropertyName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return 0.0;
}

const std::string& NFCKernelModule::GetPropertyString(const NFIDENTID& self, const std::string& strPropertyName)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetPropertyString(strPropertyName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return NULL_STR;
}

NFIDENTID NFCKernelModule::GetPropertyObject(const NFIDENTID& self, const std::string& strPropertyName)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetPropertyObject(strPropertyName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strPropertyName + "| There is no object", __FUNCTION__, __LINE__);

    return 0;
}

NFIRecord* NFCKernelModule::FindRecord(const NFIDENTID& self, const std::string& strRecordName)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordManager()->GetElement(strRecordName);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

    return NULL;
}

bool NFCKernelModule::ClearRecord(const NFIDENTID& self, const std::string& strRecordName)
{
    NFIRecord* pRecord = FindRecord(self, strRecordName);
    if (pRecord)
    {
        return pRecord->Clear();
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no record", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const int nValue)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        if (!pObject->SetRecordInt(strRecordName, nRow, nCol, nValue))
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error for row or col", __FUNCTION__, __LINE__);
        }
        else
        {
            return true;
        }
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetRecordInt( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const int value )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        if (!pObject->SetRecordInt(strRecordName, nRow, strColTag, value))
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error for row or col", __FUNCTION__, __LINE__);
        }
        else
        {
            return true;
        }
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);
    
    return false;
}

bool NFCKernelModule::SetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol,  const float fValue)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        if (!pObject->SetRecordFloat(strRecordName, nRow, nCol, fValue))
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordFloat for row  or col", __FUNCTION__, __LINE__);
        }
        else
        {
            return true;
        }
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetRecordFloat( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const float value )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        if (!pObject->SetRecordFloat(strRecordName, nRow, strColTag, value))
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordFloat for row  or col", __FUNCTION__, __LINE__);
        }
        else
        {
            return true;
        }
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName + "| There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        if (!pObject->SetRecordDouble(strRecordName, nRow, nCol, dwValue))
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordDouble for row  or col", __FUNCTION__, __LINE__);
        }
        else
        {
            return true;
        }
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetRecordDouble( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        if (!pObject->SetRecordDouble(strRecordName, nRow, strColTag, value))
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordDouble for row  or col", __FUNCTION__, __LINE__);
        }
        else
        {
            return true;
        }
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        if (!pObject->SetRecordString(strRecordName, nRow, nCol, strValue))
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordString for row  or col", __FUNCTION__, __LINE__);
        }
        else
        {
            return true;
        }
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetRecordString( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        if (!pObject->SetRecordString(strRecordName, nRow, strColTag, value))
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordObject for row  or col", __FUNCTION__, __LINE__);
        }
        else
        {
            return true;
        }
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& objectValue)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        if (!pObject->SetRecordObject(strRecordName, nRow, nCol, objectValue))
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordObject for row  or col", __FUNCTION__, __LINE__);
        }
        else
        {
            return true;
        }
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::SetRecordObject( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFIDENTID& value )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        if (!pObject->SetRecordObject(strRecordName, nRow, strColTag, value))
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, strRecordName, "error SetRecordObject for row  or col", __FUNCTION__, __LINE__);
        }
        else
        {
            return true;
        }
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return false;
}

int NFCKernelModule::GetRecordInt(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordInt(strRecordName, nRow, nCol);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return 0;
}

int NFCKernelModule::GetRecordInt( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordInt(strRecordName, nRow, strColTag);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return 0;
}

float NFCKernelModule::GetRecordFloat(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordFloat(strRecordName, nRow, nCol);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return 0.0f;
}

float NFCKernelModule::GetRecordFloat( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordFloat(strRecordName, nRow, strColTag);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return 0.0f;
}

double NFCKernelModule::GetRecordDouble(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordDouble(strRecordName, nRow, nCol);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return 0.0;
}

double NFCKernelModule::GetRecordDouble( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordDouble(strRecordName, nRow, strColTag);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return 0.0;
}

const std::string& NFCKernelModule::GetRecordString(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordString(strRecordName, nRow, nCol);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return NULL_STR;
}

const std::string& NFCKernelModule::GetRecordString( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordString(strRecordName, nRow, strColTag);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return NULL_STR;
}

NFIDENTID NFCKernelModule::GetRecordObject(const NFIDENTID& self, const std::string& strRecordName, const int nRow, const int nCol)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordObject(strRecordName, nRow, nCol);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object",  __FUNCTION__, __LINE__);

    return 0;
}

NFIDENTID NFCKernelModule::GetRecordObject( const NFIDENTID& self, const std::string& strRecordName, const int nRow, const std::string& strColTag )
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        return pObject->GetRecordObject(strRecordName, nRow, strColTag);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object",  __FUNCTION__, __LINE__);

    return 0;
}

bool NFCKernelModule::SwitchScene(const NFIDENTID& self, const int nTargetSceneID, const int nTargetGroupID, const float fX, const float fY, const float fZ, const float fOrient, const NFIValueList& arg)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        int nOldSceneID = pObject->GetPropertyInt("SceneID");
        int nOldGroupID = pObject->GetPropertyInt("GroupID");

        NFCContainerInfo* pOldSceneInfo = m_pContainerModule->GetElement(nOldSceneID);
        NFCContainerInfo* pNewSceneInfo = m_pContainerModule->GetElement(nTargetSceneID);
        if (!pOldSceneInfo
            || !pNewSceneInfo)
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, self, "no this container", nTargetSceneID);
            return false;
        }

        pOldSceneInfo->RemoveObjectFromGroup(nOldGroupID, self);

        if (nTargetSceneID != nOldSceneID)
        {
            //真的切场景
            //先退回到0层，才能修改场景ID
            pObject->SetPropertyInt("GroupID", 0);

            pObject->SetPropertyInt("SceneID", nTargetSceneID);
            //进新的场景0层
        }

        pObject->SetPropertyFloat("X", fX);
        pObject->SetPropertyFloat("Y", fY);
        pObject->SetPropertyFloat("Z", fZ);


        //相等的场景则不用切换到0层
        //到目标场景新线
        if (!pNewSceneInfo->AddObjectToGroup(nTargetGroupID, self))
        {
            return false;
        }

        pObject->SetPropertyInt("GroupID", nTargetGroupID);

        return true;
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object",  __FUNCTION__, __LINE__);

    return false;
}

NFIDENTID NFCKernelModule::CreateContainer(const int nContainerIndex, const std::string& strSceneConfigID)
{
    std::string strClassName = "Scene";
    NFIDENTID sceneIdent;

    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerIndex);
    if (pSceneInfo)
    {
        return 0;
    }

    int nWidth = 0;
    NFIPropertyManager* pConfigPropertyManager = m_pElementInfoModule->GetPropertyManager(strSceneConfigID);
    if (NULL != pConfigPropertyManager)
    {
        NFIProperty* pProperty = pConfigPropertyManager->GetElement("Width");
        if (!pProperty)
        {
            return 0;
        }

        nWidth = pProperty->GetInt();
    }


    //容器nSceneIndex
    pSceneInfo = NF_NEW NFCContainerInfo(nContainerIndex, nWidth);
    if (NULL != pSceneInfo)
    {
        m_pContainerModule->AddElement(nContainerIndex, pSceneInfo);

        //默认分组0
        NFCContainerGroupInfo* pGroupInfo = NF_NEW NFCContainerGroupInfo(nContainerIndex, nWidth);
        if (NULL != pGroupInfo)
        {
            pSceneInfo->AddElement(0, pGroupInfo);
            //容器的话，传入容器index()
            NFCValueList arg;
            arg << nContainerIndex;
            arg << nContainerIndex;

            NFIObject* pContainer = CreateObject(0, nContainerIndex, -1, strClassName, strSceneConfigID, arg);
            if (pContainer)
            {
                sceneIdent = pContainer->Self();
                pSceneInfo->SetObjectSelf(sceneIdent);
            }
        }
    }

    return sceneIdent;
}

bool NFCKernelModule::DestroyContainer(const int nContainerIndex)
{
    // also delete group scene

    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerIndex);
    if (pSceneInfo)
    {
        NFIDENTID ident = pSceneInfo->GetObjectSelf();
        NFIObject* pSceneObject = RemoveElement(ident);
        if (pSceneObject)
        {
            delete pSceneObject;
        }

    }
    NFCContainerInfo* pInfo = m_pContainerModule->RemoveElement(nContainerIndex);
    if (pInfo)
    {
        delete pInfo;
    }

    return true;
}

int NFCKernelModule::GetOnLineCount()
{
    int nCount = 0;
    NFCContainerInfo* pSceneInfo = m_pContainerModule->First();
    while (pSceneInfo)
    {
        NFCContainerGroupInfo* pGroupInfo = pSceneInfo->First();
        while (pGroupInfo)
        {
            nCount += pGroupInfo->Count();
            pGroupInfo = pSceneInfo->Next();
        }

        pSceneInfo = m_pContainerModule->Next();
    }

    return nCount;
}

int NFCKernelModule::GetMaxOnLineCount()
{
    // test count 5000
    // and it should be define in a xml file

    return 5000;
}

int NFCKernelModule::GetContainerOnLineCount(const int nContainerID)
{
    int nCount = 0;

    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (pSceneInfo)
    {
        NFCContainerGroupInfo* pGroupInfo = pSceneInfo->First();
        while (pGroupInfo)
        {
            nCount += pGroupInfo->Count();
            pGroupInfo = pSceneInfo->Next();
        }
    }

    return nCount;
}

int NFCKernelModule::GetContainerOnLineCount(const int nContainerID, const int nGroupID)
{
    int nCount = 0;

    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (pSceneInfo)
    {
        NFCContainerGroupInfo* pGroupInfo = pSceneInfo->GetElement(nGroupID);
        if (pGroupInfo)
        {
            nCount = pGroupInfo->Count();
        }
    }

    return nCount;
}

//int NFCKernelModule::GetContainerOnLineList( const int nContainerID, type, NFIValueList& var )
int NFCKernelModule::GetContainerOnLineList(const int nContainerID, NFIValueList& var)
{
    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (pSceneInfo)
    {
        NFCContainerGroupInfo* pGroupInfo = pSceneInfo->First();
        while (pGroupInfo)
        {
            NFIDENTID ident;

            bool bRet = pGroupInfo->First(ident);
            while (bRet)
            {
                var.AddObject(ident);

                bRet = pGroupInfo->Next(ident);
            }

            pGroupInfo = pSceneInfo->Next();
        }
    }

    return var.GetCount();
}

int NFCKernelModule::RequestGroupScene(const int nContainerID)
{
    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (NULL != pSceneInfo)
    {
        int nNewGroupID = pSceneInfo->NewGroupID();
        NFCContainerGroupInfo* pGroupInfo = NF_NEW NFCContainerGroupInfo(nContainerID, pSceneInfo->GetWidth());
        if (NULL != pGroupInfo)
        {
            pSceneInfo->AddElement(nNewGroupID, pGroupInfo);
            return nNewGroupID;
        }
    }

    return -1;
}

bool NFCKernelModule::ReleaseGroupScene(const int nContainerID, const int nGroupID)
{
    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (NULL != pSceneInfo)
    {
        if (pSceneInfo->GetElement(nGroupID))
        {
            // release all  object in this group
            //
            NFCValueList listObject;
            if (GetGroupObjectList(nContainerID, nGroupID, listObject))
            {
                /////////////////////////////这里不能有人在里面，否则不能释放/////////////////////////////////////////////
                //m_pLogModule->LogObject()

                for (int i = 0; i < listObject.GetCount(); i++)
                {
                    SetPropertyInt(listObject.ObjectVal(i), "GroupID", 0);
                }
            }

            NFCContainerGroupInfo* pInfo = pSceneInfo->RemoveElement(nGroupID);
            if (pInfo)
            {
                delete pInfo;
            }

            return true;//pSceneInfo->DeleteElement( nGroupID );
        }
    }

    return false;
}

bool NFCKernelModule::GetGroupObjectList(const int nContainerID, const int nGroupID, NFIValueList& list)
{
    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (pSceneInfo)
    {

        NFCContainerGroupInfo* pGroupInfo = pSceneInfo->GetElement(nGroupID);
        if (pGroupInfo)
        {
            NFIDENTID ident = 0;
            bool bRet = pGroupInfo->First(ident);
            while (bRet)
            {
                list.AddObject(ident);

                bRet = pGroupInfo->Next(ident);
            }

            return true;
        }
    }

    return false;
}

bool NFCKernelModule::GetGridObjectList(const int nContainerID, const int nGroupID, const NFIDENTID nGridID, NFIValueList& list)
{
    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (pSceneInfo)
    {
        NFCContainerGroupInfo* pGroupInfo = pSceneInfo->GetElement(nGroupID);
        if (pGroupInfo)
        {
            NFIGridModule* pGridModule =  pGroupInfo->GetGridModule();
            if (pGridModule)
            {
                return pGridModule->GetAroundObject(nGridID, list, EGRID_SELF) > 0 ? true : false;
            }
        }
    }

    return false;
}

bool NFCKernelModule::GetAroundGrid(const int nContainerID, const int nGroupID, const NFIDENTID nGridID, NFIValueList& list)
{
    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (pSceneInfo)
    {
        NFCContainerGroupInfo* pGroupInfo = pSceneInfo->GetElement(nGroupID);
        if (pGroupInfo)
        {
            NFIGridModule* pGridModule =  pGroupInfo->GetGridModule();
            if (pGridModule)
            {
                return pGridModule->GetAroundGrid(nGridID, list, EGRID_AROUND_9) > 0 ? true : false;
            }
        }
    }

    return false;
}

bool NFCKernelModule::GetRangObjectList(const NFIDENTID& self, const int nContainerID, const int nGroupID, const float fRang, NFIValueList& list)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        float fX = pObject->GetPropertyManager()->GetElement("X")->GetFloat();
        float fY = pObject->GetPropertyManager()->GetElement("Y")->GetFloat();
        float fZ = pObject->GetPropertyManager()->GetElement("Z")->GetFloat();

        // collect info
        return GetRangObjectList(fX, fY, fZ, nContainerID, nGroupID, fRang, list);
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return false;
}

bool NFCKernelModule::GetRangObjectList(const float fX, const float fY, const float fZ, const int nContainerID, const int nGroupID, const float fRang, NFIValueList& list)
{
    NFIDENTID gridIdent = NFCGridModule::ComputerGridID(fX, fY, fZ);
    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerID);
    if (pSceneInfo)
    {
        NFCContainerGroupInfo* pGroupInfo = pSceneInfo->GetElement(nGroupID);
        if (pGroupInfo)
        {
            NFIGridModule* pGridModule =  pGroupInfo->GetGridModule();
            if (pGridModule)
            {
                return pGridModule->GetAroundObject(gridIdent, list) > 0 ? true : false;
            }
        }
    }

    return false;
}

NFIDENTID NFCKernelModule::NewIdentID()
{
    // nSerial = time[013 09 12 ]+ serverID[99]+[]
    // the nSerial will be changed when the nIdent is fulled a day is passed.
    mnIdentIndex.nIdent++;
    //if (mnIdentIndex.nIdent < 0)
    //{
    //    mnIdentIndex.nSerial *= 10;
    //    mnIdentIndex.nIdent = 0;
    //}
    return mnIdentIndex;
}

bool NFCKernelModule::LogStack()
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_RED | FOREGROUND_INTENSITY);

    //NFCallStack::CALLSTACK_PTR call = NFCallStack::generate();
    //if (call)
    //{
    //    int i = 0;
    //    NFCallStack::CONST_ITERATOR it = call->Begin();
    //    for (; it != call->End(); ++it)
    //    {
    //        if (i > 2)
    //        {
    //            std::string strCallStack(*it);
    //            if (std::string::npos != strCallStack.find("NFCNet")
    //                    || std::string::npos != strCallStack.find("main"))
    //            {
    //                break;
    //            }

    //            std::string strHead = strCallStack.substr(0, strCallStack.find("()") + 2);
    //            strHead += "  line: " + strCallStack.substr(strCallStack.rfind('.') + 4, strCallStack.length());

    //            std::cout << strHead << std::endl;
    //            mLogFile << strHead << std::endl;
    //        }

    //        i++;
    //    }
    //}

    // stack walker
    //NFStackWalker sw;
    //sw.ShowCallstack();

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                            FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    return true;
}

bool NFCKernelModule::LogInfo(const NFIDENTID ident)
{
    //看是容器还是普通对象，容器则打印所有对象
    NFIObject* pObject = GetObject(ident);
    if (pObject)
    {
        if (IsContainer(ident))
        {
            int nContainerID = GetPropertyInt(ident, "SceneID");

            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "//----------child object list-------- SceneID = ", nContainerID);

            NFCValueList valObjectList;
            int nCount = GetContainerOnLineList(nContainerID, valObjectList);
            for (int i  = 0; i < nCount; i++)
            {
                NFIDENTID targetIdent = valObjectList.ObjectVal(i);
                LogInfo(targetIdent);
            }
        }
        else
        {
            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "//---------printf object start-------- ", "");


            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, ident, "//---------printf object end-------- ", "");
        }
    }
    else
    {
        m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, ident, "", __FUNCTION__, __LINE__);
    }

    return true;
}

int NFCKernelModule::OnPropertyCommonEvent(const NFIDENTID& self, const std::string& strPropertyName, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& argVar)
{
    if (IsContainer(self))
    {
        return 0;
    }

    std::list<PROPERTY_EVENT_FUNCTOR_PTR>::iterator it = mtCommonPropertyCallBackList.begin();
    for (it; it != mtCommonPropertyCallBackList.end(); it++)
    {
        PROPERTY_EVENT_FUNCTOR_PTR pFunPtr = *it;
        PROPERTY_EVENT_FUNCTOR* pFun = pFunPtr.get();
        pFun->operator()(self, strPropertyName, oldVar, newVar, argVar);
    }

    return 0;
}

NFIObject* NFCKernelModule::GetObject(const NFIDENTID& ident)
{
    return GetElement(ident);
}

bool NFCKernelModule::IsContainer(const NFIDENTID& self)
{
    NFIObject* pObject = GetObject(self);
    if (pObject)
    {
        if (pObject->GetPropertyInt("GroupID") < 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    m_pLogModule->LogObject(NFILogModule::NLL_ERROR_NORMAL, self, "There is no object", __FUNCTION__, __LINE__);

    return false;
}

int NFCKernelModule::Command(const NFIValueList& var)
{
    if (var.GetCount() > 0 && TDATA_STRING ==  var.Type(0))
    {
        std::string strCommand = var.StringVal(0);
        if ("queryObject" == strCommand && 3 == var.GetCount())
        {
            const std::string& strObjectIdent = var.StringVal(1);
            const std::string& strObjectSerial = var.StringVal(2);

            NFIDENTID ident;
            ident.nIdent = atoi(strObjectIdent.c_str());
            ident.nSerial = atoi(strObjectSerial.c_str());

            LogInfo(ident);

        }
        else if ("queryObjectProperty" == strCommand)
        {
            if (2 == var.GetCount())
            {
            }
            else
            {

            }
        }
    }

    return 0;
}

int NFCKernelModule::GetObjectByProperty(const int nContainerID, const std::string& strPropertyName, const NFIValueList& valueArg, NFIValueList& list)
{
    NFCValueList varObjectList;
    GetContainerOnLineList(nContainerID, varObjectList);
    int nWorldCount = varObjectList.GetCount();
    for (int i = 0; i < nWorldCount; i++)
    {
        NFIDENTID ident = varObjectList.ObjectVal(i);
        if (this->FindProperty(ident, strPropertyName))
        {
            TDATA_TYPE eType = valueArg.Type(0);
            switch (eType)
            {
                case TDATA_INT:
                {
                    int nValue = GetPropertyInt(ident, strPropertyName.c_str());
                    if (valueArg.IntVal(0) == nValue)
                    {
                        list.AddObject(ident);
                    }
                }
                break;
                case TDATA_STRING:
                {
                    std::string strValue = GetPropertyString(ident, strPropertyName.c_str());
                    std::string strCompareValue = valueArg.StringVal(0);
                    if (strValue == strCompareValue)
                    {
                        list.AddObject(ident);
                    }
                }
                break;
                case TDATA_OBJECT:
                {
                    NFIDENTID identObject = GetPropertyObject(ident, strPropertyName.c_str());
                    if (valueArg.ObjectVal(0) == identObject)
                    {
                        list.AddObject(ident);
                    }
                }
                break;
                default:
                    break;
            }
        }
    }

    return list.GetCount();
}

bool NFCKernelModule::ExistContainer(const int nContainerIndex)
{
    NFCContainerInfo* pSceneInfo = m_pContainerModule->GetElement(nContainerIndex);
    if (pSceneInfo)
    {
        return true;
    }

    return false;
}

bool NFCKernelModule::DestroySelf(const NFIDENTID& self)
{
    mtDeleteSelfList.push_back(self);
    return true;
}

int NFCKernelModule::OnRecordCommonEvent(const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar, const NFIValueList& arg)
{
    if (IsContainer(self))
    {
        return 0;
    }

    std::list<RECORD_EVENT_FUNCTOR_PTR>::iterator it = mtCommonRecordCallBackList.begin();
    for (it; it != mtCommonRecordCallBackList.end(); it++)
    {
        RECORD_EVENT_FUNCTOR_PTR pFunPtr = *it;
        RECORD_EVENT_FUNCTOR* pFun = pFunPtr.get();
        pFun->operator()(self, strRecordName, nOpType, nRow, nCol, oldVar, newVar, arg);
    }

    return 0;
}

int NFCKernelModule::OnClassCommonEvent(const NFIDENTID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& var)
{
    if (IsContainer(self))
    {
        return 0;
    }

    std::list<CLASS_EVENT_FUNCTOR_PTR>::iterator it = mtCommonClassCallBackList.begin();
    for (it; it != mtCommonClassCallBackList.end(); it++)
    {
        CLASS_EVENT_FUNCTOR_PTR pFunPtr = *it;
        CLASS_EVENT_FUNCTOR* pFun = pFunPtr.get();
        pFun->operator()(self, strClassName, eClassEvent, var);
    }

    return 0;
}

bool NFCKernelModule::ResgisterCommonClassEvent(const CLASS_EVENT_FUNCTOR_PTR& cb)
{
    mtCommonClassCallBackList.push_back(cb);
    return true;
}

bool NFCKernelModule::ResgisterCommonPropertyEvent(const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
    mtCommonPropertyCallBackList.push_back(cb);
    return true;
}

bool NFCKernelModule::ResgisterCommonRecordEvent(const RECORD_EVENT_FUNCTOR_PTR& cb)
{
    mtCommonRecordCallBackList.push_back(cb);
    return true;
}

bool NFCKernelModule::LogSelfInfo(const NFIDENTID ident)
{

    return false;
}

bool NFCKernelModule::AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic , bool bPrivate , bool bSave, int nIndex, const std::string& strScriptFunction)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        pObject->GetPropertyManager()->AddProperty(self, strPropertyName, varType, bPublic, bPrivate, bSave, nIndex, strScriptFunction);

        //通用回调，方便NET同步
        PROPERTY_EVENT_FUNCTOR functor = boost::bind(&NFCKernelModule::OnPropertyCommonEvent, this, _1, _2, _3, _4, _5);
        PROPERTY_EVENT_FUNCTOR_PTR functorPtr(NF_NEW PROPERTY_EVENT_FUNCTOR(functor));
        return pObject->AddPropertyCallBack(strPropertyName, functorPtr);
    }

    return false;
}

bool NFCKernelModule::AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIValueList& TData, const NFIValueList& varKey, const NFIValueList& varDesc, const NFIValueList& varTag, const NFIValueList& varRelatedRecord, const int nRows, bool bPublic, bool bPrivate, bool bSave, int nIndex)
{
    NFIObject* pObject = GetElement(self);
    if (pObject)
    {
        pObject->GetRecordManager()->AddRecord(self, strRecordName, TData, varKey, varDesc, varTag, varRelatedRecord, nRows, bPublic, bPrivate, bSave, nIndex);

        //通用回调，方便NET同步
        RECORD_EVENT_FUNCTOR functor = boost::bind(&NFCKernelModule::OnRecordCommonEvent, this, _1, _2, _3, _4, _5, _6, _7, _8);
        RECORD_EVENT_FUNCTOR_PTR functorPtr(NF_NEW RECORD_EVENT_FUNCTOR(functor));
        return pObject->AddRecordCallBack(strRecordName, functorPtr);
    }

    return false;
}

void NFCKernelModule::SetIdentSerialID(int nSerialID)
{
    mnIdentIndex = NFIDENTID(0, (NFINT16)nSerialID, 0);
}

void NFCKernelModule::SetIdentID(NFINT32 nID)
{
    mnIdentID = nID;
}

NFINT32 NFCKernelModule::GetIdentID()
{
    return mnIdentID;
}

NFIDENTID NFCKernelModule::GetGridID(const float fX, const float fY, const float fZ)
{
    return NFCGridModule::ComputerGridID((int)fX, (int)fY, (int)fZ);
}

bool NFCKernelModule::AfterInit()
{
    NFILogicClass* pClass = m_pLogicClassModule->First();
    while (pClass != NULL)
    {
        m_pEventProcessModule->AddClassCallBack(pClass->GetClassName(), this, &NFCKernelModule::OnClassCommonEvent);
        pClass = m_pLogicClassModule->Next();
    }

    return true;
}

bool NFCKernelModule::DestroyAll()
{
    NFIObject* pObject = First();
    while (pObject)
    {
        mtDeleteSelfList.push_back(pObject->Self());

        pObject = Next();
    }

    // 为了释放object
    Execute(0.1f, 0.1f);

    return true;
}

bool NFCKernelModule::BeforeShut()
{
    DestroyAll();
    return true;
}

void NFCKernelModule::Random(int nStart, int nEnd, int nCount, NFIValueList& valueList)
{
    if (mnRandomPos + nCount >= mvRandom.size())
    {
        mnRandomPos = 0;
    }

    for (int i = mnRandomPos; i < mnRandomPos + nCount; i++)
    {
        float fRanValue = mvRandom.at(i);
        int nValue = (nEnd - nStart) * fRanValue + nStart;
        valueList.AddInt((NFINT32)nValue);
    }

    mnRandomPos += nCount;
}

int NFCKernelModule::GetAllContainerObjectList( NFIValueList& var )
{
    NFCContainerInfo* pSceneInfo = m_pContainerModule->First();
    while (pSceneInfo)
    {
        NFCContainerGroupInfo* pGroupInfo = pSceneInfo->First();
        while (pGroupInfo)
        {
            NFIDENTID ident;

            bool bRet = pGroupInfo->First(ident);
            while (bRet)
            {
                var.AddObject(ident);

                bRet = pGroupInfo->Next(ident);
            }

            pGroupInfo = pSceneInfo->Next();
        }

        pSceneInfo = m_pContainerModule->Next();
    }
    return 0;
}

bool NFCKernelModule::AddEventCallBack( const NFIDENTID& self, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb )
{
    return m_pEventProcessModule->AddEventCallBack(self, nEventID, cb);
}

bool NFCKernelModule::AddClassCallBack( const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb )
{
    return m_pEventProcessModule->AddClassCallBack(strClassName, cb);
}

// bool NFCKernelModule::ResgisterCommonHeartBeat( const HEART_BEAT_FUNCTOR_PTR& cb )
// {
//     mtCommonHeartBeatCallBackList.push_back(cb);
//     
//     return true;
// }
// 
// bool NFCKernelModule::ResgisterCommonEvent( const EVENT_PROCESS_FUNCTOR_PTR& cb )
// {
//     mtCommonEventCallBackList.push_back(cb);
// 
//     return true;
// }

// int NFCKernelModule::OnHeartBeatCommonCB( const NFIDENTID& self, const std::string& strHeartBeat, const float fTime, const int nCount, const NFIValueList& var )
// {
//     if (IsContainer(self))
//     {
//         return 0;
//     }
// 
//     std::list<HEART_BEAT_FUNCTOR_PTR>::iterator it = mtCommonHeartBeatCallBackList.begin();
//     for (it; it != mtCommonHeartBeatCallBackList.end(); it++)
//     {
//         HEART_BEAT_FUNCTOR_PTR pFunPtr = *it;
//         HEART_BEAT_FUNCTOR* pFun = pFunPtr.get();
//         pFun->operator()(self, strHeartBeat, fTime, nCount, var);
//     }
// 
//     return 0;
// }
// 
// int NFCKernelModule::OnEventCommonCB( const NFIDENTID& self, const int nEventID, const NFIValueList& var )
// {
//     if (IsContainer(self))
//     {
//         return 0;
//     }
// 
//     std::list<EVENT_PROCESS_FUNCTOR_PTR>::iterator it = mtCommonEventCallBackList.begin();
//     for (it; it != mtCommonEventCallBackList.end(); it++)
//     {
//         EVENT_PROCESS_FUNCTOR_PTR pFunPtr = *it;
//         EVENT_PROCESS_FUNCTOR* pFun = pFunPtr.get();
//         pFun->operator()(self, nEventID, var);
//     }
// 
//     return 0;
// }

bool NFCKernelModule::AddRecordCallBack( const NFIDENTID& self, const std::string& strRecordName, const RECORD_EVENT_FUNCTOR_PTR& cb )
{
    NFIObject* pObject = GetObject(self);
    if (pObject)
    {
        return pObject->AddRecordCallBack(strRecordName, cb);
    }

    return false;
}

bool NFCKernelModule::AddPropertyCallBack( const NFIDENTID& self, const std::string& strPropertyName, const PROPERTY_EVENT_FUNCTOR_PTR& cb )
{
    NFIObject* pObject = GetObject(self);
    if (pObject)
    {
        return pObject->AddPropertyCallBack(strPropertyName, cb);
    }

    return false;
}

bool NFCKernelModule::AddHeartBeat( const NFIDENTID& self, const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const NFIValueList& var, const float fTime, const int nCount )
{
    NFIObject* pObject = GetObject(self);
    if(pObject)
    {
        //共有回调添加后，删除时也要删除
        //ResgisterCommonHeartBeat(cb);

        pObject->AddHeartBeat(strHeartBeatName, cb, var, fTime, nCount);
    }

    return false;
}
