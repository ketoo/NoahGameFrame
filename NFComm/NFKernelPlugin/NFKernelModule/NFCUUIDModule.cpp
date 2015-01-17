#include "NFCUUIDModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

namespace UUIDModule
{

#ifndef _MSC_VER
#include <sys/time.h>
#include <unistd.h>
#define EPOCHFILETIME 11644473600000000ULL
#else
#include <windows.h>
#include <time.h>
#define EPOCHFILETIME 11644473600000000Ui64
#endif

uint64_t get_time()
{
#ifndef _MSC_VER
    struct timeval tv;
    gettimeofday(&tv, NULL);
    int time = tv.tv_usec;
    time /= 1000;
    time += (tv.tv_sec * 1000);
    return time;
#else
    FILETIME filetime;
    uint64_t time = 0;
    GetSystemTimeAsFileTime(&filetime);

    time |= filetime.dwHighDateTime;
    time <<= 32;
    time |= filetime.dwLowDateTime;

    time /= 10;
    time -= EPOCHFILETIME;
    return time / 1000;
#endif
}

class UUID
{
public:
    UUID() : epoch_(0), time_(0), machine_(0), sequence_(0)
    {
    }

    ~UUID() {}

    void set_epoch(uint64_t epoch)
    {
        epoch_ = epoch;
    }

    void set_machine(int32_t machine)
    {
        machine_ = machine;
    }

    int64_t generate()
    {
        int64_t value = 0;
        uint64_t time = UUIDModule::get_time();

        // 保留后48位时间
        value = time << 16;

        // 最后16位是sequenceID
        value |= sequence_++;

        if (sequence_ == 0x7FFF)
        {
            sequence_ = 0;
        }

        return value;
    }
private:
    uint64_t epoch_;
    uint64_t time_;
    int32_t machine_;
    int32_t sequence_;
};
}

NFCUUIDModule::NFCUUIDModule(NFIPluginManager* p)
{
    mnIdent = 0;
	m_pKernelModule = NULL;
    pPluginManager = p;
}

bool NFCUUIDModule::Init()
{
    m_pUUID = NF_NEW UUIDModule::UUID;
    assert(NULL != m_pUUID);

    return true;
}

bool NFCUUIDModule::Shut()
{
    return true;
}

bool NFCUUIDModule::BeforeShut()
{
    if (NULL != m_pUUID)
    {
        delete m_pUUID;
        m_pUUID = NULL;
    }
    return true;
}

bool NFCUUIDModule::AfterInit()
{
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    assert(NULL != m_pKernelModule);

    // 初始化uuid
    NFINT32 nID = GetIdentID();
    m_pUUID->set_machine(nID);
    m_pUUID->set_epoch(uint64_t(1367505795100));

    return true;
}

bool NFCUUIDModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return true;
}

NFIDENTID NFCUUIDModule::CreateGUID()
{
	NFIDENTID xID;
	xID.nSvrID = GetIdentID();
	xID.nData64 = m_pUUID->generate();
	
    return xID;
}

NFINT64 NFCUUIDModule::GetIdentID()
{
    return mnIdent;
}

void NFCUUIDModule::SetIdentID( NFINT64 nID )
{
    mnIdent = nID;
}
