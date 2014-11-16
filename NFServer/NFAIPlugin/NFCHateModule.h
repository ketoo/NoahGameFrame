// -------------------------------------------------------------------------
//    文件名      ：    NFCHateModule.h
//    创建者      ：    LvSheng.Huang
//    创建时间    ：    2012-07-14 08:51
//    功能描述    ：    仇恨管理模块
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
    //仇恨列表是否为空
    virtual bool Empty(const NFIDENTID& self);

    //添加仇恨对象
    virtual bool AddHate(const NFIDENTID& self, const NFIDENTID& other, const int nValue);

    //设置仇恨值
    virtual bool SetHateValue(const NFIDENTID& self, const NFIDENTID& other, const int nValue);

    //把他人的仇恨copy到自己身上,BOSS,小弟
    virtual bool CopyHateList(const NFIDENTID& self, const NFIDENTID& other);

    //对仇恨排序，最高在前面
    virtual void CompSortList(const NFIDENTID& self);

    //得到最高仇恨的一个对象
    virtual NFIDENTID QueryMaxHateObject(const NFIDENTID& self);

    //得到最高仇恨的数个对象
    virtual bool QueryMaxHateObject(const NFIDENTID& self, std::list<std::string>& HateList, const int nCount = 1);

    //查询仇恨值
    virtual int QueryHateValue(const NFIDENTID& self, const NFIDENTID& other);

protected:

    typedef std::map<NFIDENTID, int> THATEMAP;
    typedef std::map<NFIDENTID, THATEMAP*> TOBJECTHATEMAP;

    TOBJECTHATEMAP mtObjectHateMap;

private:
};

#endif