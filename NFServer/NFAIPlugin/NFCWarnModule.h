// -------------------------------------------------------------------------
//    �ļ���      ��    NFCWarnModule.h
//    ������      ��    LvSheng.Huang
//    ����ʱ��    ��    2012-07-14 08:50
//    ��������    ��    ����������ģ��
//
// -------------------------------------------------------------------------

#ifndef _NFC_WARN_MODULE_H_
#define _NFC_WARN_MODULE_H_

#include <string>
#include <list>
#include <map>
#include <iostream>
#include "NFIWarnModule.h"

class NFCWarnModule
    : public NFIWarnModule
{
public:
    NFCWarnModule();
    virtual ~NFCWarnModule();

    //��龯����󣬿��Կ���Ϳ���
    virtual bool CheckWarnObject(const NFIDENTID& self);

    virtual void OnSpring(const NFIDENTID& self, const NFIDENTID& other);

    virtual void OnEndSpring(const NFIDENTID& self, const NFIDENTID& other);

    virtual void OnBeKilled(const NFIDENTID& self, const NFIDENTID& other);

    virtual bool OnCreateAIObject(const NFIDENTID& self);

    virtual bool OnDelAIObject(const NFIDENTID& self);

    virtual bool Empty(const NFIDENTID& self);

protected:

    bool Exist(const NFIDENTID& self);
    bool AddWarn(const NFIDENTID& self, const NFIDENTID& other);
    bool ClearWarn(const NFIDENTID& self, const NFIDENTID& other);
    bool ClearWarnAll(const NFIDENTID& self);
    bool ClearWarnAll();


private:

    //     �κ��߼����в���ʹ��map list��֮�����Ϣ��¼���κ���Ϣ��¼���������������
    //     typedef std::list<NFIDENTID> TWARNLIST;
    //     typedef std::map<NFIDENTID, TWARNLIST*> TOBJECTWARN;
    //     TOBJECTWARN mtObjectWarn;
};

#endif