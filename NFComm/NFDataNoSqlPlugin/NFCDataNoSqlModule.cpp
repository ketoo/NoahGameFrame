// -------------------------------------------------------------------------
//    @FileName      :    NFCDataNoSqlModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDataNoSqlModule
//    Row,Col; ; ; ; ; ; ;
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include <algorithm>
#include "NFCDataNoSqlModule.h"

NFCDataNoSqlModule::NFCDataNoSqlModule(NFIPluginManager* p)
{
    pPluginManager = p;
}

NFCDataNoSqlModule::~NFCDataNoSqlModule()
{

}

bool NFCDataNoSqlModule::Init()
{
    m_pPlayerDataDriver = new NFCDataNoSqlDriver();
	m_pPlayerDataDriver->Connect("127.0.0.1");

    return true;
}

bool NFCDataNoSqlModule::Shut()
{
    if (m_pPlayerDataDriver)
    {
        delete m_pPlayerDataDriver;
        m_pPlayerDataDriver = NULL;
    }

    return true;
}

int NFCDataNoSqlModule::ExistRole(const std::string& strRoleName)
{
    return 0;//m_pPlayerDataDriver->Exists(BedeckRoleProperty(strRoleName));
}

int NFCDataNoSqlModule::CreateRole(const std::string& strAccount, const std::string& strRoleName)
{
//     //创建角色需要在数个地方创建
//     //u_ROLENAME_PHT//角色属性哈希表
//     //ur_ROLENAME_XXX//角色某个record哈希表,可能很多
//     if (m_pPlayerDataDriver->Exists(BedeckRoleProperty(strRoleName)) <= 0)
//     {
//         if (m_pPlayerDataDriver->HMSet(BedeckRoleProperty(strRoleName), NFCValueList() << "RoleName", NFCValueList() << strRoleName.c_str()) > 0)
//         {
//             //此区服内帐号属性
//             //al_ACCOUNTNAME_PHT//此区服内帐号属性
//             //al_ACCOUNTNAME_RLIST//此区服内此帐号的角色列表
//             return m_pPlayerDataDriver->SAdd(BedeckAccountRoleList(strAccount), strRoleName);
//         }
//     }


    return 0;
}

bool NFCDataNoSqlModule::AfterInit()
{


    return true;
}

int NFCDataNoSqlModule::QueryAccountProperty(const std::string& strAccount, NFIValueList& valueListKeys, NFIValueList& valueListValues)
{
    return 0;//m_pPlayerDataDriver->HGetAll(BedeckAccountProperty(strAccount), valueListKeys, valueListValues);
}

int NFCDataNoSqlModule::QueryRoleProperty(const std::string& strRoleName, NFIValueList& valueListKeys, NFIValueList& valueListValues)
{
    return 0;//m_pPlayerDataDriver->HGetAll(BedeckRoleProperty(strRoleName), valueListKeys, valueListValues);
}

int NFCDataNoSqlModule::QueryRoleRecord(const std::string& strRoleName, const std::string& strRecordName, NFIValueList& valueListKeys, NFIValueList& valueListValues)
{
    return 0;//m_pPlayerDataDriver->HGetAll(BedeckRoleRecord(strRoleName, strRecordName), valueListKeys, valueListValues);
}

int NFCDataNoSqlModule::SetAccountProperty(const std::string& strAccount, const NFIValueList& valueListKeys, const NFIValueList& valueListValues)
{
    if (valueListKeys.GetCount() == valueListValues.GetCount())
    {
        return 0;//m_pPlayerDataDriver->HMSet(BedeckAccountProperty(strAccount), valueListKeys, valueListValues);
    }

    return 0;
}

int NFCDataNoSqlModule::SetRoleProperty(const std::string& strRoleName, const NFIValueList& valueListKeys, const NFIValueList& valueListValues)
{
    if (valueListKeys.GetCount() == valueListValues.GetCount())
    {
        return 0;//m_pPlayerDataDriver->HMSet(BedeckRoleProperty(strRoleName), valueListKeys, valueListValues);
    }

    return 0;
}

int NFCDataNoSqlModule::SetRoleRecord(const std::string& strRoleName, const std::string& strRecordName, const NFIValueList& valueListKeys, const NFIValueList& valueListValues)
{
    if (valueListKeys.GetCount() == valueListValues.GetCount())
    {
        return 0;//m_pPlayerDataDriver->HMSet(BedeckRoleRecord(strRoleName, strRecordName), valueListKeys, valueListValues);
    }

    return 0;
}

int NFCDataNoSqlModule::QueryAccountRoleList(const std::string& strAccount, NFIValueList& value)
{

    //m_pPlayerDataDriver->SMembers(BedeckAccountRoleList(strAccount), value);

    return value.GetCount();
}

bool NFCDataNoSqlModule::Execute(const float fLasFrametime, const float fStartedTime)
{


    return true;
}

int NFCDataNoSqlModule::DeleteRole(const std::string& strAccount, const std::string& strRoleName)
{
    //创建角色需要在数个地方创建
    //u_ROLENAME_PHT//角色属性哈希表
    //ur_ROLENAME_XXX//角色某个record哈希表,可能很多
    //m_pPlayerDataDriver->(BedeckRoleProperty(strRoleName), "RoleName", strRoleName);
    //m_pPlayerDataDriver->HValuess()
    //删除所有匹配的
    //此区服内帐号属性
    //al_ACCOUNTNAME_PHT//此区服内帐号属性
    //al_ACCOUNTNAME_RLIST//此区服内此帐号的角色列表
    return 0;//m_pPlayerDataDriver->SRemove(BedeckAccountRoleList(strAccount), NFCValueList() << strRoleName);
}

void NFCDataNoSqlModule::SavePlayerData()
{
    m_pPlayerDataDriver->Save();
}
