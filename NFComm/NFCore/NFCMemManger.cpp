#include "NFCMemManger.h"

#ifdef NF_USE_TCMALLOC
#include "Dependencies/gperftools-2.1/src/gperftools/malloc_extension.h"
#endif

NFCMemManger::NFCMemManger()
{

}

NFCMemManger::~NFCMemManger()
{

}

void NFCMemManger::FreeMem()
{

#ifdef NF_USE_TCMALLOC
    // »ØÊÕÄÚ´æ
    MallocExtension::instance()->ReleaseFreeMemory();
#endif //NF_USE_TCMALLOC

}
