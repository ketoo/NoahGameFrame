// -------------------------------------------------------------------------
//    @FileName         :    NFMemory.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-11
//    @Module           :    NFMemory
//
// -------------------------------------------------------------------------
#include <stdlib.h>
#include "NFCMemory.h"

NFCMemory xMemory;
typedef std::map<int64_t, MemoryInfo>::iterator iterator;
std::map<int64_t, MemoryInfo> _map;

NFCMemory::NFCMemory()
{
}

NFCMemory::~NFCMemory()
{
}

void NFCMemory::Dump()
{
    if (_map.size() != 0)
    {
        std::cout << _map.size() << " memory leaks detected" << std::endl;
        for (iterator itor = _map.begin(); itor != _map.end(); ++itor)
        {
            char const* file = itor->second.File();
            int line = itor->second.Line();
            std::cout << file << ", " << line << std::endl;
        }
    }
}

void NFCMemory::Add(void* p, const char* file, int line)
{
    //_map.insert(std::map<void *, MemoryInfo>::value_type(p, MemoryInfo(file, line)));
    _map[(int64_t)p] = MemoryInfo(file, line);
}

void NFCMemory::Remove(void* p)
{
    iterator itor = _map.find((int64_t)p);
    if (itor != _map.end())
    {
        _map.erase(itor);
    }
}

#if defined (NF_DEBUG_MODE) && defined(USE_NF_NEW)
void* operator new(size_t size, const char* file, int line)
{
    void* p = malloc(size);
    xMemory.Add(p, file, line);
    return p;
}

void operator delete(void* p, const char* file, int line)
{
    xMemory.Remove(p);
    free(p);
}

void* operator new(size_t size)
{
    void* p = malloc(size);
    xMemory.Add(p, "?", 0);
    return p;
}

void operator delete(void* p)
{
    xMemory.Remove(p);
    free(p);
}

void* operator new [](size_t size, const char* file, int line)
{
    void* p = malloc(size);
    xMemory.Add(p, file, line);
    return p;
}

void operator delete[](void* p, const char* file, int line)
{
    xMemory.Remove(p);
    free(p);
}

void* operator new[](size_t size)
{
    void* p = malloc(size);
    xMemory.Add(p, "?", 0);
    return p;
}

void operator delete[](void* p)
{
    xMemory.Remove(p);
    free(p);
}
#endif