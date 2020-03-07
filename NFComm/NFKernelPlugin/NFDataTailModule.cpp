/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "NFDataTailModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

#if NF_PLATFORM != NF_PLATFORM_WIN
#include <execinfo.h>
#endif

bool NFDataTailModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    return true;
}


bool NFDataTailModule::Shut()
{
    return true;
}

bool NFDataTailModule::Execute()
{
    return true;
}

bool NFDataTailModule::AfterInit()
{
#ifdef NF_DEBUG_MODE
    m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFDataTailModule::OnClassObjectEvent);
#endif
    return true;
}

int NFDataTailModule::OnClassObjectEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    if (CLASS_OBJECT_EVENT::COE_CREATE_AFTER_ATTACHDATA == eClassEvent)
	{
	    TrailObjectData(self);
	}

    std::ostringstream stream;
    switch (eClassEvent)
    {
        case CLASS_OBJECT_EVENT::COE_CREATE_NODATA:
        stream << " " + strClassName + " COE_CREATE_NODATA";
        break;
        case CLASS_OBJECT_EVENT::COE_CREATE_BEFORE_ATTACHDATA:
        stream << " " + strClassName + " COE_CREATE_BEFORE_ATTACHDATA";
        break;
        case CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA:
        stream << " " + strClassName + " COE_CREATE_LOADDATA";
        break;
        case CLASS_OBJECT_EVENT::COE_CREATE_AFTER_ATTACHDATA:
        stream << " " + strClassName + " COE_CREATE_AFTER_ATTACHDATA";
        break;
        case CLASS_OBJECT_EVENT::COE_CREATE_BEFORE_EFFECT:
        stream << " " + strClassName + " COE_CREATE_BEFORE_EFFECT";
        break;
        case CLASS_OBJECT_EVENT::COE_CREATE_EFFECTDATA:
        stream << " " + strClassName + " COE_CREATE_EFFECTDATA";
        break;
        case CLASS_OBJECT_EVENT::COE_CREATE_AFTER_EFFECT:
        stream << " " + strClassName + " COE_CREATE_AFTER_EFFECT";
        break;
        case CLASS_OBJECT_EVENT::COE_CREATE_HASDATA:
        stream << " " + strClassName + " COE_CREATE_HASDATA";
        break;
        case CLASS_OBJECT_EVENT::COE_CREATE_FINISH:
        stream << " " + strClassName + " COE_CREATE_FINISH";
        break;
        case CLASS_OBJECT_EVENT::COE_CREATE_CLIENT_FINISH:
        stream << " " + strClassName + " COE_CREATE_CLIENT_FINISH";
        break;
        case CLASS_OBJECT_EVENT::COE_BEFOREDESTROY:
        stream << " " + strClassName + " COE_BEFOREDESTROY";
        break;
        case CLASS_OBJECT_EVENT::COE_DESTROY:
        stream << " " + strClassName + " COE_DESTROY";
        break;
    }

    //m_pLogModule->LogDebug(self, stream.str());
    return 0;
}

void NFDataTailModule::StartTrail(const NFGUID& self)
{
	TrailObjectData(self);


}

void NFDataTailModule::LogObjectData(const NFGUID& self)
{
    NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->GetObject(self);
    if (nullptr == xObject)
    {
        return;
    }

    NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = xObject->GetPropertyManager();
    if (nullptr != xPropertyManager)
    {
        NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->First();
        while (nullptr != xProperty)
        {
            std::ostringstream stream;

            stream << " Start trail ";
            stream << xProperty->ToString();

            m_pLogModule->LogProperty(NFILogModule::NF_LOG_LEVEL::NLL_DEBUG_NORMAL, self, xProperty->GetKey(), stream.str(),  __FUNCTION__, __LINE__);

            xProperty = xPropertyManager->Next();
        }
    }

    NF_SHARE_PTR<NFIRecordManager> xRecordManager = xObject->GetRecordManager();
    if (nullptr != xRecordManager)
    {
        NF_SHARE_PTR<NFIRecord> xRecord = xRecordManager->First();
        while (nullptr != xRecord)
        {
            for (int i = 0; i < xRecord->GetRows(); ++i)
            {
                NFDataList xDataList;
                bool bRet = xRecord->QueryRow(i, xDataList);
                if (bRet)
                {
                    std::ostringstream stream;
                    stream << " Start trail Row[" << i << "]";

                    for (int j = 0; j < xDataList.GetCount(); ++j)
                    {
                        stream << " [" << j << "] " << xDataList.ToString(j);
                    }

                    m_pLogModule->LogRecord(NFILogModule::NF_LOG_LEVEL::NLL_DEBUG_NORMAL, self, xRecord->GetName(), stream.str(),  __FUNCTION__, __LINE__);
                }
            }

            xRecord = xRecordManager->Next();
        }
    }
}

int NFDataTailModule::OnObjectPropertyEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
    std::ostringstream stream;

    stream << strPropertyName;
    stream << ":";
    stream << oldVar.ToString();
    stream << "==>";
    stream << newVar.ToString();

    //m_pLogModule->LogDebug(self, stream.str());

    PrintStackTrace();

    return 0;
}

int NFDataTailModule::OnObjectRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
    std::ostringstream stream;
    NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(self, xEventData.strRecordName);
    if (nullptr == xRecord)
    {
        return 0;
    }

    switch (xEventData.nOpType)
    {
        case RECORD_EVENT_DATA::Add:
        {
            NFDataList xDataList;
            bool bRet = xRecord->QueryRow(xEventData.nRow, xDataList);
            if (bRet)
            {
                stream << xRecord->GetName();
                stream << " Add Row[" << xEventData.nRow << "]";

                for (int j = 0; j < xDataList.GetCount(); ++j)
                {
                    stream << " [" << j << "] " << xDataList.ToString(j);
                }

                //m_pLogModule->LogDebug(self, stream.str());

                PrintStackTrace();
            }
        }
        break;
        case RECORD_EVENT_DATA::Del:
        {
            stream << xRecord->GetName();
            stream << " Del Row[" << xEventData.nRow << "]";
            //m_pLogModule->LogDebug(self, stream.str());

            PrintStackTrace();
        }
        break;
        case RECORD_EVENT_DATA::Swap:
        {
            stream << xRecord->GetName();
            stream << " Swap Row[" << xEventData.nRow << "] Row[" << xEventData.nCol << "]";
            //m_pLogModule->LogDebug(self, stream.str());
        }
        break;
        case RECORD_EVENT_DATA::Create:
            break;
        case RECORD_EVENT_DATA::Update:
        {
            stream << xRecord->GetName();
            stream << " UpData Row[" << xEventData.nRow << "] Col[" << xEventData.nCol << "]";
            stream << oldVar.ToString();
            stream << "==>" << newVar.ToString();
            //m_pLogModule->LogDebug(self, stream.str());

            PrintStackTrace();
        }
        break;
        case RECORD_EVENT_DATA::Cleared:
            break;
        case RECORD_EVENT_DATA::Sort:
            break;
        default:
            break;
    }

    return 0;
}

int NFDataTailModule::TrailObjectData(const NFGUID& self)
{
    NF_SHARE_PTR<NFIObject> xObject = m_pKernelModule->GetObject(self);
    if (nullptr == xObject)
    {
        return -1;
    }

    NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = xObject->GetPropertyManager();
    if (nullptr != xPropertyManager)
    {
        NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->First();
        while (nullptr != xProperty)
        {
            m_pKernelModule->AddPropertyCallBack(self, xProperty->GetKey(), this, &NFDataTailModule::OnObjectPropertyEvent);

            xProperty = xPropertyManager->Next();
        }
    }

    NF_SHARE_PTR<NFIRecordManager> xRecordManager = xObject->GetRecordManager();
    if (nullptr != xRecordManager)
    {
        NF_SHARE_PTR<NFIRecord> xRecord = xRecordManager->First();
        while (nullptr != xRecord)
        {
            m_pKernelModule->AddRecordCallBack(self, xRecord->GetName(), this, &NFDataTailModule::OnObjectRecordEvent);


            xRecord = xRecordManager->Next();
        }
    }

    return 0;
}

void NFDataTailModule::PrintStackTrace()
{
    return;
#if NF_PLATFORM != NF_PLATFORM_WIN

	int size = 16;
	void * array[16];
	int stack_num = backtrace(array, size);
	char ** stacktrace = backtrace_symbols(array, stack_num);
	for (int i = 0; i < stack_num; ++i)
	{
		//printf("%s\n", stacktrace[i]);
		m_pLogModule->LogProperty(NFILogModule::NF_LOG_LEVEL::NLL_DEBUG_NORMAL, NFGUID(), "", stacktrace[i], __FUNCTION__, __LINE__);
	}

	free(stacktrace);
#endif

}