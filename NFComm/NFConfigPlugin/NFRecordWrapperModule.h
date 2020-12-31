#ifndef NFI_RECORD_READ_WRAPPER_H
#define NFI_RECORD_READ_WRAPPER_H

#include "NFComm/NFPluginModule/NFIRecrodWrapperModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFCore/NFRecordManager.h"

class NFRecordReadWrapperModule:public NFIRecordWrapper{
public:
    NFRecordReadWrapperModule(NFIPluginManager* p);
    virtual ~NFRecordReadWrapperModule();
    virtual bool Awake() override ;
    virtual NFData ReadRecordIniDataByName(string recordName);
    virtual NF_SHARE_PTR<NFIRecord> FindRecord(string className,string recordName);
private:
    NFIClassModule* m_pClassModule;
    NFIPluginManager * m_pPluginManger;
};

#endif