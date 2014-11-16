// -------------------------------------------------------------------------
//    �ļ���      ��    NFCHateModule.h
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2012-07-14 08:51
//    ��������    ��    ��޹���ģ��
//
// -------------------------------------------------------------------------

#ifndef _NFC_HATE_MODULE_H_
#define _NFC_HATE_MODULE_H_

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

    virtual bool ClearHate(const NFIDENTID& self);
    //����б��Ƿ�Ϊ��
    virtual bool Empty(const NFIDENTID& self);

    //��ӳ�޶���
    virtual bool AddHate(const NFIDENTID& self, const NFIDENTID& other, const int nValue);

    //���ó��ֵ
    virtual bool SetHateValue(const NFIDENTID& self, const NFIDENTID& other, const int nValue);

    //�����˵ĳ��copy���Լ�����,BOSS,С��
    virtual bool CopyHateList(const NFIDENTID& self, const NFIDENTID& other);

    //�Գ�����������ǰ��
    virtual void CompSortList(const NFIDENTID& self);

    //�õ���߳�޵�һ������
    virtual NFIDENTID QueryMaxHateObject(const NFIDENTID& self);

    //�õ���߳�޵���������
    virtual bool QueryMaxHateObject(const NFIDENTID& self, std::list<std::string>& HateList, const int nCount = 1);

    //��ѯ���ֵ
    virtual int QueryHateValue(const NFIDENTID& self, const NFIDENTID& other);

protected:

    typedef std::map<NFIDENTID, int> THATEMAP;
    typedef std::map<NFIDENTID, THATEMAP*> TOBJECTHATEMAP;

    TOBJECTHATEMAP mtObjectHateMap;

private:
};

#endif