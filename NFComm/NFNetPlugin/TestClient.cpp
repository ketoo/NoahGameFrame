
#include "NFCNet.h"
#include <thread>
#include <string>
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"NFNet_d.lib")

class TestClientClass
{
public:
    TestClientClass()
    {
        pNet = new NFCNet(this, &TestClientClass::ReciveHandler, &TestClientClass::EventHandler);
        pNet->Initialization("127.0.0.1", 8088);
		bConnected = false;
    }

    void ReciveHandler(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
    {
        std::string str;
        str.assign(msg, nLen);

        std::cout << " fd: " << nSockIndex << " msg_id: " << nMsgID /*<<  " data: " << str */ << " thread_id: " << std::this_thread::get_id() << std::endl;
    };

    void EventHandler(const NFSOCK nSockIndex, const NF_NET_EVENT e, NFINet* p)
    {
        std::cout << " fd: " << nSockIndex << " event_id: " << e << " thread_id: " << std::this_thread::get_id() << std::endl;
		if(e == NF_NET_EVENT_CONNECTED)
		{
			bConnected = true;
		}
    }

    void Execute()
    {
		if(bConnected)
		{
			pNet->SendMsgWithOutHead(1, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890", 100, 0);
		}

        pNet->Execute();
    }

protected:
    NFINet* pNet;
	bool bConnected;
private:
};

int main(int argc, char** argv)
{
    std::list<TestClientClass*> list;

    for (int i = 0; i < 1000; ++i)
    {
        TestClientClass* x = new TestClientClass();;
        list.push_back(x);
    }

    while (1)
    {
        std::list<TestClientClass*>::iterator it = list.begin();
        for (it; it != list.end(); ++it)
        {
            Sleep(1);

            (*it)->Execute();
        }
    }



    return 0;
}
