// -------------------------------------------------------------------------
//    �ļ���      ��    NFCHateModule.h
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2012-07-14 08:51
//    ��������    ��    ��޹���ģ��
//
// -------------------------------------------------------------------------

#ifndef NFC_HATE_MODULE_H
#define NFC_HATE_MODULE_H

#include <string>
#include <iostream>
#include <map>
#include "NFIHateModule.h"

class NFIPluginManager;

class NFCHateModule
    : public NFIHateModule
{
public:
    NFCHateModule(NFIPluginManager* pm)
    {
        pPluginManager = pm;
    }

    virtual bool ClearHate(const NFGUID& self);
    //����б��Ƿ�Ϊ��
    virtual bool Empty(const NFGUID& self);

    //��ӳ�޶���
    virtual bool AddHate(const NFGUID& self, const NFGUID& other, const int nValue);

    //���ó��ֵ
    virtual bool SetHateValue(const NFGUID& self, const NFGUID& other, const int nValue);

    //�����˵ĳ��copy���Լ�����,BOSS,С��
    virtual bool CopyHateList(const NFGUID& self, const NFGUID& other);

    //�Գ�����������ǰ��
    virtual void CompSortList(const NFGUID& self);

    //�õ���߳�޵�һ������
    virtual const NFGUID& QueryMaxHateObject(const NFGUID& self);

    //�õ���߳�޵���������
    virtual bool QueryMaxHateObject(const NFGUID& self, std::list<std::string>& HateList, const int nCount = 1);

    //��ѯ���ֵ
    virtual int QueryHateValue(const NFGUID& self, const NFGUID& other);

protected:

    typedef std::map<NFGUID, int> THATEMAP;
    typedef std::map<NFGUID, THATEMAP*> TOBJECTHATEMAP;

    TOBJECTHATEMAP mtObjectHateMap;

private:
};

#endif