// -------------------------------------------------------------------------
//    @FileName      :    NFCDataBaseNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCDataBaseNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCDataBaseNet_ClientModule.h"
#include "NFDataBaseNet_ClientPlugin.h"

extern NFIPluginManager* pPluginManager;

int NFCDataBaseNet_ClientModule::mnDataBaseID = 0;
char* NFCDataBaseNet_ClientModule::mstrsShareName = "Global\\Player";

NFIShareMemoryModule* NFCDataBaseNet_ClientModule::m_pShareMemoryModule = NULL;
NFILogicClassModule* NFCDataBaseNet_ClientModule::m_pLogicClassModule = NULL;
NFIDataBaseModule* NFCDataBaseNet_ClientModule::m_pDataBaseModule = NULL;
NFIKernelModule* NFCDataBaseNet_ClientModule::m_pKernelModule = NULL;

NFCDataBaseNet_ClientModule* NFCDataBaseNet_ClientModule::m_pThis = NULL;

bool NFCDataBaseNet_ClientModule::Init()
{
    m_pThis = this;
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
    m_pShareMemoryModule = dynamic_cast<NFIShareMemoryModule*>(pPluginManager->FindModule("NFCShareMemoryModule"));
    m_pDataBaseModule = dynamic_cast<NFIDataBaseModule*>(pPluginManager->FindModule("NFCDataBaseModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));

    assert(NULL != m_pShareMemoryModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pDataBaseModule);
    assert(NULL != m_pKernelModule);

    // 连接master server
    Initialization(false, "NFDataBaseNet_ClientPlugin.cfg");



    return true;
}

bool NFCDataBaseNet_ClientModule::Shut()
{
    UnRegister();

    Final();

    return true;
}

bool NFCDataBaseNet_ClientModule::DispatcherSystemEvents(RakNet::MessageID ucEventID, RakNet::SystemAddress sAddress)
{
    switch (ucEventID)
    {
        case ID_CONNECTION_REQUEST_ACCEPTED:
        {
            printf("NFClient_Net connect successed %s\n", sAddress.ToString());
            Register();
        }
        break;
        case ID_CONNECTION_LOST:
        {
            printf("Client server close %s\n", sAddress.ToString());
        }
        break;
        case ID_DISCONNECTION_NOTIFICATION:
        {
            printf("NFClient_Net disconnected %s\n", sAddress.ToString());
        }
        break;
        default:
            break;
    }
    return true;
}

bool NFCDataBaseNet_ClientModule::DispatcherPacket(NFIPacket* pMsgPacket)
{
    unsigned int unMsgID = pMsgPacket->GetMessageId();
    switch (unMsgID)
    {
        case MTD_LOAD_ROLEDATA:
            LoadRoleDataProcess(pMsgPacket);
            break;

        case MTD_SAVE_ROLEDATA:
            SaveRoleDataProcess(pMsgPacket);
            break;

        default:
            printf("NFNet || 非法消息:unMsgID=%d\n", pMsgPacket->GetMessageId());
            break;
    }

    return true;
}

bool NFCDataBaseNet_ClientModule::Execute(const float fLasFrametime, const float fStartedTime)
{
#ifdef _DEBUG

    char szContent[MAX_PATH] = { 0 };
    if (kbhit() && gets(szContent))
    {
        NFCValueList val(szContent, ",");
        if (val.GetCount() > 0)
        {
            const char* pstrCmd = val.StringVal(0);
            if (0 == strcmp("queryMemory", pstrCmd))
            {
                if (4 == val.GetCount())
                {
                    //共享名，用户名
                    m_pShareMemoryModule->PrintElementInfo(val.StringVal(1), val.StringVal(2), atoi(val.StringVal(3)));
                }
            }
        }
    }

#endif

    return m_pThis->Update(fLasFrametime, fStartedTime);
}

bool NFCDataBaseNet_ClientModule::AfterInit()
{
    CreateShareMemory();

    return true;
}

void NFCDataBaseNet_ClientModule::LoadRoleDataProcess(NFIPacket* pMsgPacket)
{
    _tagMTD_LOAD_ROLEDATA loadData;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    loadData.DeCode(*pBitStream);

    int nPlayerConfig[4] = { 0 };
    NFCValueList valueRoleList;
    if (m_pDataBaseModule->QueryAccountRoleList(loadData._szAccountName, valueRoleList) > 0)
    {
        for (int i = 0; i < valueRoleList.GetCount(); i++)
        {
            nPlayerConfig[i] = LoadRoleData(valueRoleList.StringVal(i));
            if (nPlayerConfig[i] > 0)
            {
                m_pKernelModule->LogInfo("LoadRoleData:");
                m_pKernelModule->LogInfo(valueRoleList.StringVal(i));
            }
            else
            {
                nPlayerConfig[i] = 0;
            }
        }

        //load final
        _tagDTM_LOAD_ROLEDATA_FINAL loadFinal;
        strcpy(loadFinal._szAccountName, loadData._szAccountName);
        strcpy(loadFinal._szRoleName1, valueRoleList.StringVal(0));
        strcpy(loadFinal._szRoleName2, valueRoleList.StringVal(1));
        strcpy(loadFinal._szRoleName3, valueRoleList.StringVal(2));
        strcpy(loadFinal._szRoleName4, valueRoleList.StringVal(3));

        loadFinal.nPlayerConfidIndex1 = nPlayerConfig[0];
        loadFinal.nPlayerConfidIndex2 = nPlayerConfig[1];
        loadFinal.nPlayerConfidIndex3 = nPlayerConfig[2];
        loadFinal.nPlayerConfidIndex4 = nPlayerConfig[3];

        RakNet::BitStream bitStream;
        loadFinal.EnCode(bitStream);
        SendBitStream(&bitStream);
    }
}

int NFCDataBaseNet_ClientModule::LoadRoleData(const std::string& strRoleName)
{
    GTPropertyManager* pPropertyManager = m_pLogicClassModule->GetClassPropertyManager("Player");
    GTRecordManager* pRecordManager = m_pLogicClassModule->GetClassRecordManager("Player");
    if (!pPropertyManager || !pRecordManager)
    {
        return -1;
    }

    if (m_pDataBaseModule->HasRole(strRoleName) <= 0)
    {
        return -1;
    }

    if (m_pShareMemoryModule->FindUserShareElement(mstrsShareName, strRoleName, 0))
    {
        NFCValueList valueProperty;
        if (m_pDataBaseModule->QueryRoleProperty(strRoleName, valueProperty) > 0)
        {
            int nJob = 0;
            int nSex = 0;

            for (int i = 0; i < valueProperty.GetCount() ; i++)
            {
                const char* pstrPropertyInfo = valueProperty.StringVal(i);
                NFCValueList propertyValue(pstrPropertyInfo, ",");

                if (0 == strcmp(propertyValue.StringVal(0), "Job"))
                {
                    nJob = atoi(propertyValue.StringVal(1));
                }

                if (0 == strcmp(propertyValue.StringVal(0), "Sex"))
                {
                    nSex = atoi(propertyValue.StringVal(1));
                }
            }

            return nJob + nSex * 10;
        }

        return 0;
    }
    // all property and record write to share memories
    if (!m_pShareMemoryModule->RequireUserShareElement(mstrsShareName, strRoleName))
    {
        //只是初始化了人物名，并没有初始化属性名和表名
        return -1;
    }

    int nPlayerConfigIndex = LoadRoleDataProperty(strRoleName);
    LoadRoleDataRecord(strRoleName);


    return nPlayerConfigIndex;
}

void NFCDataBaseNet_ClientModule::SaveRoleDataProcess(NFIPacket* pMsgPacket)
{
    _tagMTD_SAVE_ROLEDATA saveRole;
    RakNet::BitStream* pBitStream = pMsgPacket->GetBitStream();
    saveRole.DeCode(*pBitStream);

    SaveRoleData(saveRole._szRoleName);

    m_pShareMemoryModule->ReleaseUserShareElement(mstrsShareName, saveRole._szRoleName);
}

void NFCDataBaseNet_ClientModule::CreateShareMemory()
{
    GTPropertyManager* pPropertyManager = m_pLogicClassModule->GetClassPropertyManager("Player");
    GTRecordManager* pRecordManager = m_pLogicClassModule->GetClassRecordManager("Player");

    if (pPropertyManager && pRecordManager)
    {
        int nRecordElementCount = 0;
        int nPropertyElementCount = pPropertyManager->Count();

        NFIRecord* pRecord = pRecordManager->First();
        while (pRecord)
        {
            nRecordElementCount += pRecord->GetCols() * pRecord->GetRows();

            pRecord = pRecordManager->Next();
        }

        int nCount = nRecordElementCount + nPropertyElementCount;

        m_pShareMemoryModule->CreateShareElement(mstrsShareName, nCount, 500);
    }

}

void NFCDataBaseNet_ClientModule::Register()
{
    _tagDTM_DATABASE_REGISTERED  registeredBase;
    registeredBase._nDataBaseID = 1;

    RakNet::BitStream oBitStream;
    registeredBase.EnCode(oBitStream);
    SendBitStream(&oBitStream);
}

void NFCDataBaseNet_ClientModule::UnRegister()
{
    _tagDTM_DATABASE_UNREGISTERED unRegisteredBase;
    unRegisteredBase._nDataBaseID = 1;

    RakNet::BitStream oBitStream;
    unRegisteredBase.EnCode(oBitStream);
    SendBitStream(&oBitStream);
}

bool NFCDataBaseNet_ClientModule::BeforeShut()
{
    //m_pShareMemoryModule->DestroyShareElement( mstrsShareName );
    return true;
}

bool NFCDataBaseNet_ClientModule::SaveRoleData(const std::string& strRoleName)
{
    GTPropertyManager* pPropertyManager = m_pLogicClassModule->GetClassPropertyManager("Player");
    GTRecordManager* pRecordManager = m_pLogicClassModule->GetClassRecordManager("Player");
    if (!pPropertyManager || !pRecordManager)
    {
        return true;
    }

    if (m_pDataBaseModule->HasRole(strRoleName) <= 0)
    {
        return true;
    }

    //  if ( !m_pShareMemoryModule->FindUserShareElement( mstrsShareName, saveRole._szRoleName, 0 ) )
    //  {
    //      return;
    //  }

    //属性保存

    NFCValueList valuePropertyList;

    int nPropertyCount = pPropertyManager->Count();
    for (int i = 0; i < nPropertyCount; i++)
    {
        NFCValueList::ShareElement* pShareElement = m_pShareMemoryModule->FindUserShareElement(mstrsShareName, strRoleName, i);
        if (pShareElement)
        {
            GTProperty* pProperty = pPropertyManager->GetElement(pShareElement->mstrElementName);
            if (pProperty)
            {
                if (pProperty->GetSave())
                {
                    switch (pShareElement->mData.nType)
                    {
                        case VTYPE_STRING:
                        {
                            std::string strValue = pShareElement->mstrElementName;
                            strValue.append(",");
                            strValue.append(pShareElement->mData.mstrValue);

                            valuePropertyList.AddString(strValue.c_str());
                        }
                        break;
                        case VTYPE_INT:
                        {
                            char str[MAX_PATH] = { 0 };
                            itoa(pShareElement->mData.mnValue , str, 10);

                            std::string strValue = pShareElement->mstrElementName;
                            strValue.append(",");
                            strValue.append(str);

                            valuePropertyList.AddString(strValue.c_str());
                        }
                        break;
                        case VTYPE_FLOAT:
                        {
                            char str[MAX_PATH] = { 0 };
                            sprintf(str, "%f", pShareElement->mData.mfValue);

                            std::string strValue = pShareElement->mstrElementName;
                            strValue.append(",");
                            strValue.append(str);

                            valuePropertyList.AddString(strValue.c_str());
                        }
                        break;
                        case VTYPE_DOUBLE:
                        {
                            char str[MAX_PATH] = { 0 };
                            sprintf(str, "%f", pShareElement->mData.mdValue);

                            std::string strValue = pShareElement->mstrElementName;
                            strValue.append(",");
                            strValue.append(str);

                            valuePropertyList.AddString(strValue.c_str());
                        }
                        break;
                        case VTYPE_OBJECT:
                        {
                            std::string str = NFIDENTID::ToString(pShareElement->mData.mnValue64);

                            std::string strValue = pShareElement->mstrElementName;
                            strValue.append(",");
                            strValue.append(str);

                            valuePropertyList.AddString(strValue.c_str());
                        }
                        break;
                        default:
                            break;
                    }
                }
            }
        }
    }

    m_pDataBaseModule->SetRoleProperty(strRoleName, valuePropertyList);

    //表保存
    NFCValueList valueRecordList;
    int nRecordCount = pRecordManager->Count();
    for (int i = 0; i < nRecordCount; i++)
    {
        NFCValueList::ShareElement* pShareElement = m_pShareMemoryModule->FindUserShareElement(mstrsShareName, strRoleName, i + nPropertyCount);
        if (pShareElement)
        {
            GTProperty* pProperty = pPropertyManager->GetElement(pShareElement->mstrElementName);
            if (pProperty)
            {
            }
        }
    }
    return true;
}

int NFCDataBaseNet_ClientModule::LoadRoleDataProperty(const std::string& strRoleName)
{
    int nJob = 0;
    int nSex = 0;

    GTPropertyManager* pPropertyManager = m_pLogicClassModule->GetClassPropertyManager("Player");
    GTRecordManager* pRecordManager = m_pLogicClassModule->GetClassRecordManager("Player");

    NFCValueList valueProperty;
    if (m_pDataBaseModule->QueryRoleProperty(strRoleName, valueProperty) <= 0)
    {
        //可能是没数据,人工手动创建的号
        //return false;
    }

    //每个属性拆开，设置进共享内存去[而且这里只初始化了在数据库中找到了的属性，其他的属性端均没初始化]
    for (int i = 0; i < valueProperty.GetCount() ; i++)
    {
        NFCValueList::ShareElement* pShareData = m_pShareMemoryModule->FindUserShareElement(mstrsShareName, strRoleName, i);
        if (pShareData)
        {
            const char* pstrPropertyInfo = valueProperty.StringVal(i);
            NFCValueList propertyValue(pstrPropertyInfo, ",");
            NFCValueList shareValue;

            GTProperty* pProperty = pPropertyManager->GetElement(propertyValue.StringVal(0));
            if (pProperty)
            {
                const NFIValueList& valueInit = pProperty->GetValue();

                switch (valueInit.GetType(0))
                {
                    case VARIANT_TYPE::VTYPE_STRING:
                        shareValue.AddString(propertyValue.StringVal(1));
                        break;
                    case VARIANT_TYPE::VTYPE_INT:
                    {
                        int nData = atoi(propertyValue.StringVal(1));
                        shareValue.AddInt(nData);
                    }
                    break;
                    case VARIANT_TYPE::VTYPE_FLOAT:
                    {
                        float fData = atof(propertyValue.StringVal(1));
                        shareValue.AddFloat(fData);
                    }
                    break;
                    case VARIANT_TYPE::VTYPE_DOUBLE:
                    {
                        float fData = atof(propertyValue.StringVal(1));
                        shareValue.AddFloat(fData);
                    }
                    break;
                    case VARIANT_TYPE::VTYPE_OBJECT:
                    {
                        //                         NFCValueList valueIdent( propertyValue.StringVal( 1 ), "-" );
                        //                         NFIDENTID ident( atoi( valueIdent.StringVal( 0 ) ), atoi( valueIdent.StringVal( 1 ) ) );
                        //                         shareValue.AddObject( ident );
                        NFIDENTID ident(propertyValue.StringVal(1));
                        shareValue.AddObject(ident);
                    }
                    break;
                    default:
                        break;
                }

                //找到了属性，然后看属性类型，把值设置进去
                pShareData->SetOwnerName(strRoleName.c_str());
                pShareData->SetElementName(pProperty->GetKey());
                pShareData->SetVarData(shareValue.GetVarData(0));

                if (0 == strcmp(pProperty->GetKey(), "Job"))
                {
                    nJob = atoi(propertyValue.StringVal(1));
                }

                if (0 == strcmp(pProperty->GetKey(), "Sex"))
                {
                    nSex = atoi(propertyValue.StringVal(1));
                }


                NFCValueList::ShareElement* p = m_pShareMemoryModule->FindUserShareElement(mstrsShareName, strRoleName, i);

                char szDataInfo[MAX_PATH] = { 0 };
                sprintf(szDataInfo, "%08x : %s[%s] : %s", p, p->mstrOwnerName, p->mstrElementName, propertyValue.StringVal(1));
                m_pKernelModule->LogInfo(szDataInfo);

            }
        }
    }

    for (int i = 0; i < pPropertyManager->Count(); i++)
    {
        NFCValueList::ShareElement* pShareData = m_pShareMemoryModule->FindUserShareElement(mstrsShareName, strRoleName, i);

        char szDataInfo[MAX_PATH] = { 0 };
        sprintf(szDataInfo, "%08x : %s[%s] : --", pShareData, pShareData->mstrOwnerName, pShareData->mstrElementName);
        m_pKernelModule->LogInfo(szDataInfo);
    }

    return (nJob + nSex * 10);
}

int NFCDataBaseNet_ClientModule::LoadRoleDataRecord(const std::string& strRoleName)
{
    GTPropertyManager* pPropertyManager = m_pLogicClassModule->GetClassPropertyManager("Player");
    GTRecordManager* pRecordManager = m_pLogicClassModule->GetClassRecordManager("Player");

    NFCValueList valueRecord;
    if (m_pDataBaseModule->QueryRoleRecord(strRoleName, valueRecord) <= 0)
    {
        //可能是没数据,人工手动创建的号
        //return false;
    }

    int nRecordHeart = 3;

    //上次取属性到了哪个位置的共享内存序号
    int nShareElementIndex = pPropertyManager->Count();
    //所有的表循环
    for (int i = 0; i < valueRecord.GetCount(); i++)
    {
        NFCValueList varRecordElementList;
        varRecordElementList.Split(valueRecord.StringVal(i), ",");
        if (varRecordElementList.GetCount() > nRecordHeart)
        {
            //varRecordElementList中第一个是此表的表名，第2，3分别是Row和Col数量,余下的Row*Col个是内容
            std::string strRecordName = varRecordElementList.StringVal(0);
            NFIRecord* pRecord = pRecordManager->GetElement(strRecordName);
            if (!pRecord)
            {
                assert(0);
                return -1;
            }

            int nOldRow = atoi(varRecordElementList.StringVal(1));
            int nOldCol = atoi(varRecordElementList.StringVal(2));

            int nNewRow = pRecord->GetRows();
            int nNewCol = pRecord->GetCols();
            for (int ntemRow = 0; ntemRow < nNewRow; ntemRow++)
            {
                for (int ntemCol = 0; ntemCol < nNewCol; ntemCol++)
                {
                    int ntemMemoryIndex = ntemRow * nNewCol + ntemCol;
                    NFCValueList::ShareElement* pShareData = m_pShareMemoryModule->FindUserShareElement(mstrsShareName, strRoleName, nShareElementIndex + ntemMemoryIndex);
                    if (pShareData)
                    {
                        pShareData->SetRow(ntemRow);
                        pShareData->SetCol(ntemCol);
                        pShareData->SetOwnerName(strRoleName.c_str());
                        pShareData->SetElementName(strRecordName.c_str());

                        if (ntemRow >= nOldRow)
                        {
                            //超越了数据库中的row
                            break;
                        }
                        if (ntemCol >= nOldCol)
                        {
                            //超越了数据库中的row
                            break;
                        }

                        //有多少填充多少
                        const char* pstrTableElementInfo = varRecordElementList.StringVal(ntemMemoryIndex + nRecordHeart);
                        NFCValueList shareValue;
                        VARIANT_TYPE varType = pRecord->GetColType(ntemCol);
                        switch (varType)
                        {
                            case VARIANT_TYPE::VTYPE_STRING:
                                shareValue.AddString(pstrTableElementInfo);
                                break;
                            case VARIANT_TYPE::VTYPE_INT:
                            {
                                int nData = atoi(pstrTableElementInfo);
                                shareValue.AddInt(nData);
                            }
                            break;
                            case VARIANT_TYPE::VTYPE_FLOAT:
                            {
                                float fData = atof(pstrTableElementInfo);
                                shareValue.AddFloat(fData);
                            }
                            break;
                            case VARIANT_TYPE::VTYPE_DOUBLE:
                            {
                                float fData = atof(pstrTableElementInfo);
                                shareValue.AddFloat(fData);
                            }
                            break;
                            case VARIANT_TYPE::VTYPE_OBJECT:
                            {
                                //                                 NFCValueList valueIdent( pstrTableElementInfo, "-" );
                                //                                 NFIDENTID ident( atoi( valueIdent.StringVal( 0 ) ), atoi( valueIdent.StringVal( 1 ) ) );
                                //                                 shareValue.AddObject( ident );
                                NFIDENTID ident(pstrTableElementInfo);
                                shareValue.AddObject(ident);
                            }
                            break;
                            default:
                                break;
                        }

                        //找到了属性，然后看属性类型，把值设置进去

                        pShareData->SetVarData(shareValue.GetVarData(0));

                        NFCValueList::ShareElement* p = m_pShareMemoryModule->FindUserShareElement(mstrsShareName, strRoleName, nShareElementIndex + ntemMemoryIndex);

                        char szDataInfo[MAX_PATH] = { 0 };
                        sprintf(szDataInfo, "%08x : %s[%s] :[%d*%d] %s", p, p->mstrOwnerName, p->mstrElementName, p->mnRow, p->mnCol, pstrTableElementInfo);
                        m_pKernelModule->LogInfo(szDataInfo);


                    }
                    else
                    {
                        assert(0);
                    }
                }
            }

            //递归
            nShareElementIndex += nNewRow * nNewCol;

        }
    }
    return valueRecord.GetCount();
}