// -------------------------------------------------------------------------
//    @FileName      :    NFCDynLib.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCDynLib
//
// -------------------------------------------------------------------------

#include "NFCDynLib.h"

bool NFCDynLib::Load()
{
    mInst = (DYNLIB_HANDLE)DYNLIB_LOAD(mstrName.c_str());

    return mInst != NULL;

}

bool NFCDynLib::UnLoad()
{
    DYNLIB_UNLOAD(mInst);    
    return true;
}

void* NFCDynLib::GetSymbol(const char* szProcName)
{
    return (DYNLIB_HANDLE)DYNLIB_GETSYM(mInst, szProcName);
}
