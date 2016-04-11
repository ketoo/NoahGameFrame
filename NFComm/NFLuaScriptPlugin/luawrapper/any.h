/*	
    Copyright (C) <2012>  <huangweilook@21cn.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/	
#ifndef _ANY_H
#define _ANY_H
#include <vector>
#include <iostream>
#include <stdint.h>
#include <map>
#include <stdlib.h>
#include "TypeList.h"
#include "Trait.h"
#include "luacommon.h"

namespace luacpp{

class luaObject;
class any;
typedef std::vector<any> luatable;

typedef LOKI_TYPELIST_15(bool,char,unsigned char,short,unsigned short,int,unsigned int,long,unsigned long,float,double,std::string,luaObject,luatable,int64_t) SupportType;

typedef LOKI_TYPELIST_1(std::string) STDStringType;

typedef LOKI_TYPELIST_9(char,unsigned char,short,unsigned short,int,unsigned int,long,unsigned long,int64_t) intType;

//注册到lua中的用户数据类型信息
template<typename T>
class luaRegisterClass
{
public:
	static bool isRegister()
	{
		return m_isRegister;
	}

	static void SetRegister()
	{
		m_isRegister = true;
	}

	static void ClearRegister()
	{	
		m_isRegister = false;
	}

	static void SetClassName(const char *name)
	{
		classname = name;
	}

	static const char *GetClassName()
	{
		return classname.c_str();
	}

private:
	static bool m_isRegister;
	static std::string classname;
};

template<typename T>
bool luaRegisterClass<T>::m_isRegister = false;

template<typename T>
std::string luaRegisterClass<T>::classname;

class any;
class any_pusher
{
public:
	virtual void push(lua_State *L,any*) = 0; 
};
template <typename T>
any_pusher *create_any_pusher();

//一个精简版的boost::any
class any
{
public: // structors

   any(): content(0),counter(0){}

   template<typename ValueType>
   any(const ValueType & value)
      :counter(new int(1)),luaRegisterClassName("")
   {
	   content = create_holder<ValueType>(value,Int2Type<IndexOf<intType,ValueType>::value >= 0>());
	   luaRegisterClassName = 	luaRegisterClass<typename pointerTraits<ValueType>::PointeeType>::GetClassName();
	   _any_pusher = create_any_pusher<ValueType>();
   }

   any(const any & other)
   {
	   if(other.counter && other.content)
	   {
		   content = other.content;
		   counter = other.counter;
		   ++(*counter);
		   luaRegisterClassName = other.luaRegisterClassName;
		   _any_pusher = other._any_pusher;
	   }
	   else
	   {
		   content = NULL;
		   counter = NULL;
		   _any_pusher = NULL;
	   }
   }

   	any & operator=(const any & rhs)
	{
		if(&rhs == this)
			return *this;
		else
		{      
			_destroy();
			if(rhs.counter && rhs.content)
			{
				content = rhs.content;
				counter = rhs.counter;
				_any_pusher = rhs._any_pusher;
				++(*counter);
			}
			else
			{
				content = 0;
				counter = 0;
			}
			return *this;
		}
	}
	
   ~any()
   {
		_destroy();
   }

private:
	void _destroy()
	{
		if(counter && content && --*counter <= 0)
		{
			delete counter;
			delete content;
			delete _any_pusher;
			//printf("destroy\n");
		}
	}

public: // queries
    bool empty() const
    {
       return !content;
    }
public:

    class placeholder
    {
      public: // structors
		virtual ~placeholder(){}
     };

     template<typename ValueType>
     class holder : public placeholder
     {
       public: // structors

         holder(const ValueType & value)
           : held(value)
		   {}
       public: // representation
			 ValueType held;
      };
	  
	  template<typename ValueType>
	  placeholder * create_holder(ValueType v,Int2Type<true>)
	  {
		 return new holder<int64_t>(v);
	  }
	  
	  template<typename ValueType>
	  placeholder * create_holder(ValueType v,Int2Type<false>)
	  {
		 return new holder<ValueType>(v);
	  }
	  
   placeholder * content;
   int         * counter;
public: // representation (public so any_cast can be non-friend)
   any_pusher     * _any_pusher;
   std::string luaRegisterClassName;//保存的类型在lua中注册的名字

};

template<typename ValueType>
inline ValueType _any_cast(const any &operand,Int2Type<true>)
{
	ValueType ret = (ValueType)static_cast<any::holder<int64_t> *>(operand.content)->held;
	return ret;
}

template<typename ValueType>
inline ValueType _any_cast(const any &operand,Int2Type<false>)
{
	return static_cast<any::holder<ValueType> *>(operand.content)->held;
}

template<typename ValueType>
inline ValueType any_cast(const any & operand)
{
	return _any_cast<ValueType>(operand,Int2Type<IndexOf<intType,ValueType>::value >= 0>());
}

template<>
inline std::string any_cast(const any & operand)
{
	any::holder<std::string> *tmp = static_cast<any::holder<std::string> *>(operand.content);
	return tmp->held;
}

template<>
inline const char *any_cast(const any &operand){
	any::holder<std::string> *tmp = static_cast<any::holder<std::string> *>(operand.content);
	return tmp->held.c_str();
}


template<>
inline char *any_cast(const any &operand){
	any::holder<std::string> *tmp = static_cast<any::holder<std::string> *>(operand.content);
	return const_cast<char*>(tmp->held.c_str());
}


template<>
inline double any_cast(const any & operand)
{
	any::holder<double> *tmp = static_cast<any::holder<double> *>(operand.content);
	return tmp->held;
}

template<>
inline float any_cast(const any & operand)
{
	return (float)any_cast<double>(operand);
}



//unsupported
template<>
const char * any_cast(const any & operand);

template<>
char * any_cast(const any & operand);

}
#endif
