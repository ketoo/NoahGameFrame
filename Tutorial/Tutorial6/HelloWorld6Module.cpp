#include "HelloWorld6Module.h"
#include "NFComm/NFCore/NFTimer.h"

bool HelloWorld6Module::Init()
{
    //初始化
    std::cout << "Hello, world6, Init" << std::endl;

    return true;
}

bool HelloWorld6Module::AfterInit()
{
    //初始化完毕
    std::cout << "Hello, world6, AfterInit" << std::endl;

    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    m_pLuaScriptModule = pPluginManager->FindModule<NFILuaScriptModule>("NFCLuaScriptModule");

    //assert( NULL != m_pLuaScriptModule);

    m_pKernelModule->CreateContainer(1, "");
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(NFGUID(), 1, 0, "Player", "", NFCDataList());
    if ( !pObject.get() )
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement("TaskList");
    if ( pRecord.get() )
    {
        NFCDataList var;
        var << "Task_From_C++";
        var << 0;
        pRecord->AddRow(-1, var);
    }

    pObject->SetPropertyInt("MAXHP",100);


    m_pEventProcessModule->DoEvent(pObject->Self(), 2222, NFCDataList());

    return true;
}

bool HelloWorld6Module::Execute()
{
    //每帧执行
    //std::cout << "Hello, world6, Execute" << std::endl;

    return true;
}

bool HelloWorld6Module::BeforeShut()
{
    //反初始化之前
    std::cout << "Hello, world6, BeforeShut" << std::endl;

    m_pKernelModule->DestroyAll();

    return true;
}

bool HelloWorld6Module::Shut()
{
    //反初始化
    std::cout << "Hello, world6, Shut" << std::endl;

    return true;
}
