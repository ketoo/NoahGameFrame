// -------------------------------------------------------------------------
//    @FileName			:    Tutorial7.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2018-05-19
//    @Module           :	 Tutorial7
//
// -------------------------------------------------------------------------

#ifndef NF_TUTORIAL7_H
#define NF_TUTORIAL7_H

///
#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class Tutorial7 : public NFIPlugin
{
public:
	Tutorial7(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual const int GetPluginVersion();

    virtual const std::string GetPluginName();

    virtual void Install();

    virtual void Uninstall();
};
#endif