// -------------------------------------------------------------------------
//    @FileName         :    NFMemory.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-07-11
//    @Module           :    NFMemory
//
// -------------------------------------------------------------------------

#ifndef _NFC_MEMORY_H_
#define _NFC_MEMORY_H_

#pragma pack(push, 1)

#include "NFPlatform.h"
#include "NFIMemory.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "boost/mpl/size_t_fwd.hpp"
#include <boost/lexical_cast.hpp>
#include "../mono/utils/mono-publib.h"

class MemoryInfo  
{  
public:  
    MemoryInfo (char const * file, int line)  
        : _file (file), _line (line)  
    {}  
    MemoryInfo ()  
        : _file (0), _line (0)  
    {}

    char const * File () const { return _file; }  
    int Line () const { return _line; }  
private:  
    char const * _file;  
    int _line;  
}; 

class NFCMemory : public NFIMemory 
{
public:  
    NFCMemory ();  
    virtual ~NFCMemory ();

    virtual void Add (void * p, char const * file, int line);  
    virtual void Remove (void * p);  
    virtual void Dump ();  

private:
    


};
#ifndef USE_NF_NEW
#define USE_NF_NEW
#endif // !USE_NF_NEW


#if defined (NF_DEBUG_MODE) && defined(USE_NF_NEW)
void* operator new (size_t size, const char* file, int line);
void operator delete(void* p, const char* file, int line); 

void* operator new (size_t size);
void operator delete(void* p);
void* operator new [](size_t size, const char* file, int line);
void operator delete[](void* p, const char* file, int line);
void* operator new[] (size_t size);
void operator delete[](void* p);
#endif

#if defined (NF_DEBUG_MODE) && defined(USE_NF_NEW)
#define NF_NEW new(__FILE__,__LINE__)
#else
#define NF_NEW new
#endif

#pragma pack(pop)
#endif
