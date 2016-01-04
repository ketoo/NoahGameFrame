#ifndef NFC_NFCMEMMANGER_H
#define NFC_NFCMEMMANGER_H

#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFSingleton.h"

class NFCMemManger: public NFSingleton<NFCMemManger>
{
public:
    NFCMemManger();
    ~NFCMemManger();

public:
    virtual void FreeMem();

private:

};

#endif