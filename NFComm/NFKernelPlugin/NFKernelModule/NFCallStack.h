/**
* @brief   ���Թ���
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
    * @brief ����㲻���ݲ�������ô�㽫�õ���ǰʱ�̵ĵ��ö�ջ
    *        �����ʹ����������SEH�쳣����ô�뽫LPEXCEPTION_POINTERS->ContextRecord����ȥ
    **/
    static CALLSTACK_PTR generate(const void* pContext = NULL);

private:
    std::tr1::shared_ptr<NFCallStackImp>         mspImp;
};