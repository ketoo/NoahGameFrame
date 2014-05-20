/**
* @brief   调试工具
* @author  dongbo
* @date    2010-3-17
* @remarks
**/

#pragma once

#include <list>
#include <string>
//#include "boost/config/no_tr1/memory.hpp"
//#include <boost/tr1/tr1/memory>


class NFCallStackImp;

class NFCallStack
{
public:
    typedef std::string                            FUNC_NAME;
    typedef std::list< FUNC_NAME >                 FUNC_NAME_LIST;
    typedef FUNC_NAME_LIST::const_iterator         CONST_ITERATOR;
    typedef std::tr1::shared_ptr<NFCallStack>        CALLSTACK_PTR;

    friend class NFCallStackImp;

public:
    NFCallStack();

public:
    CONST_ITERATOR  Begin() const;
    CONST_ITERATOR  End() const;

public:
    /**
    * @brief 如果你不传递参数，那么你将得到当前时刻的调用堆栈
    *        如果你使用它来处理SEH异常，那么请将LPEXCEPTION_POINTERS->ContextRecord传进去
    **/
    static CALLSTACK_PTR generate(const void* pContext = NULL);

private:
    std::tr1::shared_ptr<NFCallStackImp>         mspImp;
};