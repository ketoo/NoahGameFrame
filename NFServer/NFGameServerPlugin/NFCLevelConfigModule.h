// -------------------------------------------------------------------------
//    @FileName      :    NFCLevelConfigModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-09-28
//    @Module           :    NFCLevelConfigModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_LEVELCONFIG_MODULE_H_
#define _NFC_LEVELCONFIG_MODULE_H_

#include "NFComm/Config/NFConfig.h"
#include "NFComm/RapidXML/rapidxml.hpp"
#include "NFComm/RapidXML/rapidxml_iterators.hpp"
#include "NFComm/RapidXML/rapidxml_print.hpp"
#include "NFComm/RapidXML/rapidxml_utils.hpp"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILevelConfigModule.h"

class NFCLevelConfigModule
    : public NFILevelConfigModule,
      NFMap<int, int>
{
public:

    NFCLevelConfigModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }
    virtual ~NFCLevelConfigModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute( const float fLasFrametime, const float fStartedTime );
    virtual bool AfterInit();

    virtual int Load();

    virtual int GetNeedExp( int nLevel );

protected:

private:


};


#endif
