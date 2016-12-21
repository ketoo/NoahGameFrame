// -------------------------------------------------------------------------
//    @FileName         :    NFCMemManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCMemManager
//
// -------------------------------------------------------------------------

#include "NFCMemManager.h"

#ifdef NF_USE_TCMALLOC
#include "Dependencies/gperftools-2.1/src/gperftools/malloc_extension.h"
#endif

NFCMemManager::NFCMemManager()
{

}

NFCMemManager::~NFCMemManager()
{

}

void NFCMemManager::FreeMem()
{

#ifdef NF_USE_TCMALLOC
    
    MallocExtension::instance()->ReleaseFreeMemory();
#endif //NF_USE_TCMALLOC

}
