#include "NFCMemManger.h"
#include "Dependencies/gperftools-2.1/src/gperftools/malloc_extension.h"

NFCMemManger::NFCMemManger()
{

}

NFCMemManger::~NFCMemManger()
{

}

void NFCMemManger::FreeMem()
{
    // »ØÊÕÄÚ´æ
    MallocExtension::instance()->ReleaseFreeMemory();
}
