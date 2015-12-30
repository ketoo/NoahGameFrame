// -------------------------------------------------------------------------
//    @FileName         £º    NFIMem.h
//    @Author           £º    LvSheng.Huang
//    @Date             £º    2013-12-15
//    @Module           £º    NFIMem
//    @Desc             :     INet
// -------------------------------------------------------------------------

#ifndef NFI_MEM_H
#define NFI_MEM_H

class NFIMem
{
public:
    virtual ~NFIMem()
    {

    }

public:
    virtual void FreeMem() = 0;
};

#endif
