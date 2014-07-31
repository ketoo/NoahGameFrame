#include "NFComm/NFCore/NFPlatform.h"

/*
int main()
{

NFIDataNoSqlModule* pModule = new NFCDataNoSqlModule();
pModule->Init();

int nRet = -1;
//���ķ���������ʺ�
nRet = pModule->AddAccountInfo("test1", "123456");
//ȥ���ķ�����ȷ���ʺ�����
nRet = pModule->ConfirmAccountInfo("test1", "123456");

//��Ϸ������ȷ���ʺ�
nRet = pModule->HasAccount("test1");
//��Ϸ�����������ʺ�
nRet = pModule->CreateAccount("test1", "Password");
//////////////////////////////////////////////////////////////////////////
//��Ϸ�������ʺ��������
NFCDataList valueAccountPropertyList;
NFCDataList valueAccountPropertyValueList;
for (int i = 0; i < 10; i++)
{
char szProperty[MAX_PATH] = { 0 };
char szValue[MAX_PATH] = { 0 };
sprintf(szProperty, "Property_%d", i);
sprintf(szValue, "Value_%d", i);
valueAccountPropertyList.AddString(szProperty);
valueAccountPropertyValueList.AddString(szValue);
}

nRet = pModule->SetAccountProperty("test1", valueAccountPropertyList, valueAccountPropertyValueList);

//��Ϸ�������ʺ����Ի�ȡ
valueAccountPropertyList.Clear();
valueAccountPropertyValueList.Clear();
nRet = pModule->QueryAccountProperty("test1", valueAccountPropertyList, valueAccountPropertyValueList);
for (int i = 0; i < valueAccountPropertyList.GetCount(); i++)
{
std::cout << valueAccountPropertyList.StringVal(i) << "  " << valueAccountPropertyValueList.StringVal(i) << std::endl;
}
//////////////////////////////////////////////////////////////////////////
//��Ϸ������Ϊĳ�ʺŴ�����ɫ
nRet = pModule->CreateRole("test1", "Role1");
nRet = pModule->CreateRole("test1", "Role2");

//��Ϸ��������ô��ʺ��½�ɫ�б�
NFCDataList valueRoleList;
nRet = pModule->QueryAccountRoleList("test1", valueRoleList);
for (int i = 0; i < valueRoleList.GetCount(); i++)
{
std::cout << valueRoleList.StringVal(i) << std::endl;
}

//////////////////////////////////////////////////////////////////////////
//��Ϸ���������ý�ɫ����
NFCDataList valueRolePropertyList;
NFCDataList valueRolePropertyValueList;
for (int i = 0; i < 10; i++)
{
char szProperty[MAX_PATH] = { 0 };
char szValue[MAX_PATH] = { 0 };
sprintf(szProperty, "Property_%d", i);
sprintf(szValue, "Value_%d", i);
valueRolePropertyList.AddString(szProperty);
valueRolePropertyValueList.AddString(szValue);
}

nRet = pModule->SetRoleProperty("Role1", valueRolePropertyList, valueRolePropertyValueList);

//��Ϸ��������ȡ��ɫ�����б�
valueRolePropertyList.Clear();
valueRolePropertyValueList.Clear();
nRet = pModule->QueryRoleProperty("Role1", valueRolePropertyList, valueRolePropertyValueList);
for (int i = 0; i < valueRolePropertyList.GetCount(); i++)
{
std::cout << valueRolePropertyList.StringVal(i) << "  " << valueRolePropertyValueList.StringVal(i) << std::endl;
}

//////////////////////////////////////////////////////////////////////////
//��Ϸ���������ý�ɫ������
NFCDataList valueRoleRecordValueList;
for (int i = 0; i < 4; i++)
{
for (int j = 0; j < 7; j++)
{
valueRoleRecordValueList.AddInt(i*j);
}
}
nRet = pModule->SetRoleRecord("Role1", "record1", 4, 7, valueRoleRecordValueList);

//��Ϸ��������ȡ��ɫ������
valueRoleRecordValueList.Clear();
int nRow = 0;
int nCol = 0;
nRet = pModule->QueryRoleRecord("Role1", "record1", nRow, nCol, valueRoleRecordValueList);
for (int i = 0; i < valueRoleRecordValueList.GetCount(); i++)
{
std::cout << valueRoleRecordValueList.StringVal(i) << std::endl;
}

return 0;
}
*/
#ifdef NF_DEBUG_MODE

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "NFCore_d.lib" )
#pragma comment( lib, "anet_win64_d.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "NFCore_d.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "NFCore.lib" )
#pragma comment( lib, "anet_win64.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "NFCore.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#endif