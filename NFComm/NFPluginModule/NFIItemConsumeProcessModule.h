// -------------------------------------------------------------------------
//    @FileName      :   NFIItemConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFIItemConsumeProcessModule
//    @Desc             :   �������ѻ�����,��ϸ�ľ���ĳ��������������Լ��۳�����
// -------------------------------------------------------------------------

#ifndef _NFI_ITEMCONSUME_PROCESS_MODULE_H
#define _NFI_ITEMCONSUME_PROCESS_MODULE_H

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/Define/NFItem_def.h"
#include "NFComm\NFMessageDefine\NFDefine.pb.h"

class NFIItemConsumeProcessModule
    : public NFILogicModule
{
public:

    //��Ʒʹ���Ƿ�Ϸ�
    virtual int ConsumeLegal(const NFGUID& self, const std::string& strItemName, const NFGUID& targetID) = 0;

    //�Ϸ�,����,��ô�������[���ĺ�,nItemRowID�Ѿ��Ҳ����ˣ���Ϊ���ܱ������]
    virtual int ConsumeProcess(const NFGUID& self, const std::string& strItemName, const NFGUID& targetID) = 0;

};

#endif