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

struct coro_pck
{
    coro_pck()
    {
        nID = 0;
        _mainCoro = NULL;
        state = NFCoroutineState::READY;
    }

	ucontext_t* _mainCoro;
	int nID;

	ucontext_t taskCoro;
    enum NFCoroutineState state;
};

void Schedule();
void Execute();

void thread_work(coro_pck *pData)
{
        std::cout << "do_some_work_start ID " << pData->nID << std::endl;
        sleep(3);
        pData->state = NFCoroutineState::FINISHED;
        std::cout << "do_some_work_end ID " << pData->nID << std::endl;

}


void funckkkkkkk(void * arg)
{
	coro_pck* pck = (coro_pck*)arg;

    std::cout << "enter coro ID " << pck->nID << std::endl;

    if (pck->state == NFCoroutineState::READY)
    {
        pck->state = NFCoroutineState::RUNNING;

        std::thread t(thread_work, pck);
        t.detach();
    }

    while(1)
    {

	    //sleep(1);
        if (pck->state == NFCoroutineState::FINISHED)
        {

            pck->state = NFCoroutineState::BACKING;
            break;
        }
        else
        {
            printf("switch to main\n");

            swapcontext(&(pck->taskCoro), pck->_mainCoro);

            std::cout << "switch in coro ID " << pck->nID << std::endl;
        }

    }

    std::cout << "leave coro ID " << pck->nID << std::endl;
}

std::vector<coro_pck*> coroutine_list;

int start_coroutine(ucontext_t* ctx_main)
{

    std::cout << "start_coroutine_start" << std::endl;

    coro_pck* coroPck = new coro_pck();
    coroPck->nID = coroutine_list.size();
    coroutine_list.push_back(coroPck);

    coroPck->_mainCoro = ctx_main;


	getcontext(&coroPck->taskCoro);

	coroPck->taskCoro.uc_stack.ss_sp = malloc(1024*1024);
	coroPck->taskCoro.uc_stack.ss_size = 1024 * 1024;
	coroPck->taskCoro.uc_stack.ss_flags = 0;
	coroPck->taskCoro.uc_link = ctx_main;

	makecontext(&(coroPck->taskCoro), (void(*)(void))funckkkkkkk, 1, coroPck);

	//check state finished
    while(1)
    {

	    //sleep(1);
        if (coroPck->state == NFCoroutineState::BACKING)
        {
            coroPck->state = NFCoroutineState::FREE;
            std::cout << "FREE coro ID " << coroPck->nID << std::endl;

            break;
        }
        else
        {
            std::cout << "switch to coro ID " << coroPck->nID << std::endl;

            swapcontext(ctx_main, &(coroPck->taskCoro));

            printf("switch in main\n");

            Schedule();
        }
    }
    std::cout << "start_coroutine_end " << coroPck->nID << std::endl;

    return coroPck->nID;
}
int i = 0;

ucontext_t mainCoro;

void OtherBusiness1()
{
        if (i <= 0)
        {
            i++;

            std::cout << "use coroutine---start000" << std::endl;
            start_coroutine(&mainCoro);
            std::cout << "use coroutine---end0000" << std::endl;

            std::cout << "use coroutine---start111" << std::endl;
            start_coroutine(&mainCoro);
            std::cout << "use coroutine---end111" << std::endl;

            std::cout << "use coroutine---start222" << std::endl;
            start_coroutine(&mainCoro);
            std::cout << "use coroutine---end222" << std::endl;

        }
    printf("do others business1111111\n");
}

void OtherBusiness2()
{
    printf("do others business222222222\n");
}

void Schedule()
{
    sleep(1);

        for (int index = 0; index < coroutine_list.size(); ++index)
        {

            coro_pck* coroPck =coroutine_list[index];
            if (coroPck->state == NFCoroutineState::FINISHED)
            {

                std::cout << "Schedule switch to coro ID " << coroPck->nID << std::endl;

                swapcontext(coroPck->_mainCoro, &(coroPck->taskCoro));
                //break;
                return;
            }
        }

        Execute();
}

void Execute()
{

    printf("Execute\n");

    OtherBusiness1();
    OtherBusiness2();

    Schedule();
}

int main()
{


	while (true)
	{
        Execute();
	}

	return 0;
}
