// -------------------------------------------------------------------------
//    @FileName			:    NFCScaleModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-10-23 12:22
//    @Module           :    NFCScaleModule
//    @Desc             :
// -------------------------------------------------------------------------

#ifndef NFC_SCALE_MODULE_H
#define NFC_SCALE_MODULE_H

#include <memory>
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIScaleModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"

class NFCScaleModule
    : public NFIScaleModule
{
public:
	NFCScaleModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();

protected:

    //////////////////////////////////////////////////////////////////////////
    NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
	NFILogModule* m_pLogModule;
	NFIElementModule* m_pElementModule;
    //////////////////////////////////////////////////////////////////////////
};
#endif
