// -------------------------------------------------------------------------
//    @FileName         £º    NFCMem.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-12-15
//    @Module           £º    NFIPacket
//    @Desc             :     NFCMem
// -------------------------------------------------------------------------

#ifndef NFC_MEM_H
#define NFC_MEM_H

#include "NFIMem.h"

class NFCMem : public NFIMem
{
public:
	virtual ~NFCMem(){};

public:
    virtual void FreeMem();
};

#endif
