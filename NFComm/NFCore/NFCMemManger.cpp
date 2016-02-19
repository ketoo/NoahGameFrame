#include "NFCMemManger.h"
#include "gperftools/malloc_extension.h"

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
