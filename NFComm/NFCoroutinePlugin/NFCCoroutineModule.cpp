// -------------------------------------------------------------------------
//    @FileName			:    NFCCoroutineModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-03-07
//    @Module           :    NFCCoroutineModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCCoroutineModule.h"
#include "NFComm/NFCore/NFMapEx.hpp"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

static char g_stack[2048];
static ucontext_t ctx_main;
static NF_SHARE_PTR<NFContextData> xConTextPtr = nullptr;
//void* func(void* a_pObj)
void func()
{
    //WS_T_P_CONTROL_T* pObj = (WS_T_P_CONTROL_T*)a_pObj;
    // do something.
    cout << "enter func" << endl;

    sleep(2);

    if (xConTextPtr)
    {
        xConTextPtr->SetState(NFContextState::NFCOROUTINE_COMPLETED);

        setcontext(&ctx_main);
    }
}

int main()
{
    /*
    ucontext_t context;

    getcontext(&context);
    puts("Hello world");
    sleep(5);
    setcontext(&context);
*/
    NFCCoroutineModule xCCoroutineModule(nullptr);


    while(1)
    {
        sleep(1);
        xCCoroutineModule.Execute();

    }

    return 0;
}

NFCCoroutineModule::NFCCoroutineModule(NFIPluginManager* p)
{
	pPluginManager = p;
	mnIndex = 1;
}

NFCCoroutineModule::~NFCCoroutineModule()
{
}


bool NFCCoroutineModule::Execute()
{

    static bool bInit = false;
    if (!bInit)
    {
        bInit = true;
        getcontext(&ctx_main);
    }

    static int iindex  = 0;
    if (iindex < 3)
    {
        iindex += 1;

        std::cout << "StartCoroutine begin" << std::endl;
        StartCoroutine(&func);
        std::cout << "StartCoroutine end" << std::endl;
    }

    Schedule();



	return true;
}

void NFCCoroutineModule::Schedule()
{
	for (auto it = mxCoroutineMap.begin(); it != mxCoroutineMap.end(); it++)
	{
		switch (it->second->GetState())
		{
		case NFContextState::NFCOROUTINE_READY:
		{
			//start
			//1:synchron--exp:sleep
			//2:asynchron--exp:actor
			StartCoroutine(it->second);

		}
		break;
		case NFContextState::NFCOROUTINE_RUNNING:
		{
			if (it->second->GetOutTime() > (uint64_t)NFGetTime())
			{
				it->second->SetState(NFContextState::NFCOROUTINE_DEAD);
			}
			else
			{
				//check actor msessage
			}
		}
		break;
		case NFContextState::NFCOROUTINE_COMPLETED:
		{
			EndCoroutine(it->second);
		}
		break;

		default:
			break;
		}
	}

	for (auto it = mxCoroutineMap.begin(); it != mxCoroutineMap.end();)
	{
		if (it->second->GetState() == NFContextState::NFCOROUTINE_DEAD)
		{
			it = mxCoroutineMap.erase(it);
			continue;
		}

		it++;
	}

	return;
}

void NFCCoroutineModule::StartCoroutine(NF_SHARE_PTR<NFContextData> xContextData)
{
#ifndef _MSC_VER

	xContextData->SetState(NFContextState::NFCOROUTINE_RUNNING);
    xConTextPtr = xContextData;

    swapcontext(&ctx_main, &(xContextData->ctx_func));

#endif // _MSC_VER
}

void NFCCoroutineModule::EndCoroutine(NF_SHARE_PTR<NFContextData> xContextData)
{
	xContextData->SetState(NFContextState::NFCOROUTINE_DEAD);

    setcontext(&(xContextData->ctx_resume));
	//swap context ---continue execute last address
}

NFINT64 NFCCoroutineModule::CreateNewContextID()
{
	return mnIndex++;
}

bool NFCCoroutineModule::StartCoroutine(TaskFunction const & fn)
{
	NF_SHARE_PTR<NFContextData> xContextData(NF_NEW NFContextData(this));
	NFINT64 nContextID = CreateNewContextID();
	mxCoroutineMap[nContextID] = xContextData;

#ifndef _MSC_VER

    getcontext(&(xContextData->ctx_func));

    xContextData->ctx_func.uc_stack.ss_sp = g_stack;
    xContextData->ctx_func.uc_stack.ss_size = sizeof g_stack;
    //xCoroutine[i].ctx_func.uc_link = &ctx_main;
    xContextData->ctx_func.uc_link = NULL;
    makecontext(&(xContextData->ctx_func), func, 0);


    getcontext(&(xContextData->ctx_resume));

    if (xContextData->GetState() == NFContextState::NFCOROUTINE_READY)
    {
        setcontext(&ctx_main);
    }

    std::cout << " The End of StartCoroutine " << std::endl;
#endif

	return true;
}

bool NFCCoroutineModule::Init()
{
	return true;
}

bool NFCCoroutineModule::AfterInit()
{
	return true;
}

bool NFCCoroutineModule::Shut()
{
	return true;
}

bool NFCCoroutineModule::BeforeShut()
{
	return true;
}
