#include "NFRecordWrapperModule.h"
NFData NFRecordReadWrapperModule::ReadRecordIniDataByName(string recordName){
    return NFData();
}
NFRecordReadWrapperModule::NFRecordReadWrapperModule(NFIPluginManager *pluginManger){
    m_pPluginManger = pluginManger;
}
NFRecordReadWrapperModule::~NFRecordReadWrapperModule(){

}
bool NFRecordReadWrapperModule::Awake(){
    m_pClassModule = m_pPluginManger->FindModule<NFIClassModule>();
    return true;
}
NF_SHARE_PTR<NFIRecord> NFRecordReadWrapperModule::FindRecord(string className,string recordName){
    NF_SHARE_PTR<NFIRecordManager> classRecordManager = m_pClassModule->GetClassRecordManager(className);
    if(!classRecordManager){
        return nullptr;
    }
    return classRecordManager->GetElement(recordName);
}