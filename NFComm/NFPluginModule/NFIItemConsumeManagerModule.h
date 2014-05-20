// -------------------------------------------------------------------------
//    @FileName      :   NFIItemConsumeManagerModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFIItemConsumeManagerModule
//    @Desc             :   �������ѻ��ƹ�����,�������͵ĵ����������;���ע���������
// -------------------------------------------------------------------------

#ifndef _NFI_ITEMCONSUME_MANAGER_MODULE_H_
#define _NFI_ITEMCONSUME_MANAGER_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFIItemConsumeProcessModule.h"

class NFIItemConsumeManagerModule
    : public NFILogicModule
{

public:
    virtual bool ResgisterConsumeModule(const int nModuleType, NFIItemConsumeProcessModule* pModule) = 0;

    virtual NFIItemConsumeProcessModule* GetConsumeModule(const int nModuleType) = 0;

};

#endif