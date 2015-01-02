// -------------------------------------------------------------------------
//    @FileName      :    NFIEventRouteModule.h.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-01-02
//    @Module           :    NFIEventRouteModule.h
//    @Desc			:	�¼�·�ɣ���Ҫ��������ֲ�ʽ����Щ�����ڱ�������Ȼ����Ҫת�����¼����ʼ���
// -------------------------------------------------------------------------

#ifndef _NFI_EVENTROUTE_MODULE_H_
#define _NFI_EVENTROUTE_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"

class NFIEventRouteModule
    : public NFILogicModule
{

public:
	//�ʼ����Ӻ������룬Լռ���ȶ���������·�ɳ��¼����ٵ�Ŀ���������ԭ���߼�����
	//��ʱ���ɷ������߼�������Щ����粻��������ۼ���һ��
	virtual bool StartRouteEvent(const NFIDENTID& self, const NFIDataList& xDataList) = 0;

	virtual bool ReceiveRouteEvent(const NFIDENTID& self, const NFIDataList& xDataList) = 0;
};

#endif