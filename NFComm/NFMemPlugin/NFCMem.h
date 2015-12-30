// -------------------------------------------------------------------------
//    @FileName         ��    NFCMem.h
//    @Author           ��    LvSheng.Huang
//    @Date             ��    2013-12-15
//    @Module           ��    NFIPacket
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
