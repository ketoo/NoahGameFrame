// -------------------------------------------------------------------------
//    @FileName         :    NFSingleton.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2011-01-21
//    @Module           :    NFSingleton
//
// -------------------------------------------------------------------------

#ifndef _GT_SINGLETON_H_
#define _GT_SINGLETON_H_

#include <iostream>

template <class T>
class NFSingleton
{

public:

    NFSingleton()
    {
        //assert( !m_instance );
        // 下面是判断VC++编译器版本，版本不同相应的处理也不同
#if defined( _MSC_VER ) && _MSC_VER < 1200
        int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
        m_pInstance = (T*)((int)this + offset);
#else
        m_pInstance = static_cast< T* >(this);
#endif
    }

    ~NFSingleton()
    {
        //assert( m_instance );
        m_pInstance = NULL;
    }

public:

    static T* GetSingletonPtr()
    {
        if (NULL == m_pInstance)
        {
            m_pInstance = new T;
        }

        return m_pInstance;
    }

    static T& GetSingletonRef()
    {
        return *GetSingletonPtr();
    }

    static T* Instance()
    {
        return GetSingletonPtr();
    }

    static T& InstanceRef()
    {
        return GetSingletonRef();
    }

    static void ReleaseInstance()
    {
        if (NULL != m_pInstance)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

private:
    static T* m_pInstance;
};

template <class T>
T* NFSingleton<T>::m_pInstance = NULL;

#endif