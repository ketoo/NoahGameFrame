// -------------------------------------------------------------------------
//    @FileName      :   NFIItemConsumeProcessModule.h
//    @Author           :   LvSheng.Huang
//    @Date             :   2013-09-28
//    @Module           :   NFIItemConsumeProcessModule
//    @Desc             :   �������ѻ�����,��ϸ�ľ���ĳ��������������Լ��۳�����
// -------------------------------------------------------------------------

#ifndef _NFI_ITEMCONSUME_PROCESS_MODULE_H_
#define _NFI_ITEMCONSUME_PROCESS_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/Define/NFItem_def.h"

class NFIItemConsumeProcessModule
    : public NFILogicModule
{
public:

    //��Ʒʹ���Ƿ�Ϸ�
    virtual int ConsumeLegal(const NFIDENTID& self, int nItemRowID,  const NFIValueList& other) = 0;

    //ʹ����Ʒ������
    virtual int ConsumeSelf(const NFIDENTID& self, int nItemRowID) = 0;

    //�Ϸ�,����,��ô�������[���ĺ�,nItemRowID�Ѿ��Ҳ����ˣ���Ϊ���ܱ������]
    virtual int ConsumeProcess(const NFIDENTID& self, const std::string& strItemName, const NFIValueList& other) = 0;

};

#endif