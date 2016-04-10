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
#ifndef _LUAOBJECT_H
#define _LUAOBJECT_H
#include "luacommon.h"
#include "luaFunction.h"

#define PREPARE(L,index){\
		lua_rawgeti(L,LUA_REGISTRYINDEX,index);\
		lua_pushstring(L,name);\
		lua_gettable(L,-2);\
		lua_rawgeti(L,LUA_REGISTRYINDEX,index);\
	}

namespace luacpp {

	template<typename T>
	T popvalue(lua_State *L);

	template<typename T>
	void push_obj(lua_State *L, const T &obj);

	//代表lua中的一个对象(其实就是一个表)
	class luaObject
	{
	public:
		luaObject(lua_State *lState, int index)
			:m_iKeyIndex(index), m_plState(lState) {
			counter = new int(1);
		}

		luaObject()
			:m_iKeyIndex(0), m_plState(0), counter(NULL) {}

		luaObject &operator = (const luaObject & other)
		{
			if (this != &other)
			{
				if (!counter)
				{
					counter = other.counter;
					++(*counter);
					this->m_iKeyIndex = other.m_iKeyIndex;
					this->m_plState = other.m_plState;
				}
				else
				{
					if (--(*counter) <= 0)
					{
						//原来指向的对象现在已经没有其它对象在引用了,可以从lua中释放掉了
						luaL_unref(this->m_plState, LUA_REGISTRYINDEX, this->m_iKeyIndex);
						delete counter;
					}

					counter = other.counter;
					++(*counter);
					this->m_iKeyIndex = other.m_iKeyIndex;
					this->m_plState = other.m_plState;
				}
			}
			return *this;
		}

		luaObject(const luaObject &other)
			:m_iKeyIndex(other.m_iKeyIndex), m_plState(other.m_plState), counter(other.counter)
		{
			++(*counter);
		}

		~luaObject()
		{
			if (counter)
			{
				if (!(--(*counter)))
				{
					luaL_unref(m_plState, LUA_REGISTRYINDEX, m_iKeyIndex);
					delete counter;
				}
			}
		}


		template<typename RET, typename KEY>
		RET Get(KEY key)
		{
			lua_rawgeti(m_plState, LUA_REGISTRYINDEX, m_iKeyIndex);
			push_obj<KEY>(m_plState, key);
			lua_gettable(m_plState, -2);
			RET ret = popvalue<RET>(m_plState);
			lua_pop(m_plState, 1);//pop the table
			return ret;
		}

		template<typename T, typename KEY>
		void Set(KEY key, T value)
		{
			lua_rawgeti(m_plState, LUA_REGISTRYINDEX, m_iKeyIndex);
			push_obj<KEY>(m_plState, key);
			push_obj<T>(m_plState, value);
			lua_settable(m_plState, -3);
			lua_pop(m_plState, 1);
		}

		template<typename Ret>
		Ret call(const char *name)
		{
			PREPARE(m_plState, m_iKeyIndex);
			return doLuaCall<Ret>::doCall(m_plState, 1, 0, true);
		}

		template<typename Ret, typename Arg1>
		Ret call(const char *name, const Arg1 &arg1)
		{
			PREPARE(m_plState, m_iKeyIndex);
			push_obj<Arg1>(m_plState, arg1);
			return doLuaCall<Ret>::doCall(m_plState, 2, 0, true);
		}

		template<typename Ret, typename Arg1, typename Arg2>
		Ret call(const char *name, const Arg1 &arg1, const Arg2 &arg2)
		{
			PREPARE(m_plState, m_iKeyIndex);
			push_obj<Arg1>(m_plState, arg1);
			push_obj<Arg2>(m_plState, arg2);
			return doLuaCall<Ret>::doCall(m_plState, 3, 0, true);
		}

		template<typename Ret, typename Arg1, typename Arg2, typename Arg3>
		Ret call(const char *name, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3)
		{
			PREPARE(m_plState, m_iKeyIndex);
			push_obj<Arg1>(m_plState, arg1);
			push_obj<Arg2>(m_plState, arg2);
			push_obj<Arg3>(m_plState, arg3);
			return doLuaCall<Ret>::doCall(m_plState, 4, 0, true);
		}

		template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		Ret call(const char *name, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4)
		{
			PREPARE(m_plState, m_iKeyIndex);
			push_obj<Arg1>(m_plState, arg1);
			push_obj<Arg2>(m_plState, arg2);
			push_obj<Arg3>(m_plState, arg3);
			push_obj<Arg4>(m_plState, arg4);
			return doLuaCall<Ret>::doCall(m_plState, 5, 0, true);
		}

		template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
		Ret call(const char *name, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5)
		{
			PREPARE(m_plState, m_iKeyIndex);
			push_obj<Arg1>(m_plState, arg1);
			push_obj<Arg2>(m_plState, arg2);
			push_obj<Arg3>(m_plState, arg3);
			push_obj<Arg4>(m_plState, arg4);
			push_obj<Arg5>(m_plState, arg5);
			return doLuaCall<Ret>::doCall(m_plState, 6, 0, true);
		}

		template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6>
		Ret call(const char *name, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5, const Arg6 &arg6)
		{
			PREPARE(m_plState, m_iKeyIndex);
			push_obj<Arg1>(m_plState, arg1);
			push_obj<Arg2>(m_plState, arg2);
			push_obj<Arg3>(m_plState, arg3);
			push_obj<Arg4>(m_plState, arg4);
			push_obj<Arg5>(m_plState, arg5);
			push_obj<Arg6>(m_plState, arg6);
			return doLuaCall<Ret>::doCall(m_plState, 7, 0, true);
		}

		template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7>
		Ret call(const char *name, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5, const Arg6 &arg6, const Arg7 &arg7)
		{
			PREPARE(m_plState, m_iKeyIndex);
			push_obj<Arg1>(m_plState, arg1);
			push_obj<Arg2>(m_plState, arg2);
			push_obj<Arg3>(m_plState, arg3);
			push_obj<Arg4>(m_plState, arg4);
			push_obj<Arg5>(m_plState, arg5);
			push_obj<Arg6>(m_plState, arg6);
			push_obj<Arg7>(m_plState, arg7);
			return doLuaCall<Ret>::doCall(m_plState, 8, 0, true);
		}

		template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8>
		Ret call(const char *name, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5, const Arg6 &arg6, const Arg7 &arg7, const Arg8 &arg8)
		{
			PREPARE(m_plState, m_iKeyIndex);
			push_obj<Arg1>(m_plState, arg1);
			push_obj<Arg2>(m_plState, arg2);
			push_obj<Arg3>(m_plState, arg3);
			push_obj<Arg4>(m_plState, arg4);
			push_obj<Arg5>(m_plState, arg5);
			push_obj<Arg6>(m_plState, arg6);
			push_obj<Arg7>(m_plState, arg7);
			push_obj<Arg8>(m_plState, arg8);
			return doLuaCall<Ret>::doCall(m_plState, 9, 0, true);
		}

		template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
		Ret call(const char *name, const Arg1 &arg1, const Arg2 &arg2, const Arg3 &arg3, const Arg4 &arg4, const Arg5 &arg5, const Arg6 &arg6, const Arg7 &arg7, const Arg8 &arg8, const Arg9 &arg9)
		{
			PREPARE(m_plState, m_iKeyIndex);
			push_obj<Arg1>(m_plState, arg1);
			push_obj<Arg2>(m_plState, arg2);
			push_obj<Arg3>(m_plState, arg3);
			push_obj<Arg4>(m_plState, arg4);
			push_obj<Arg5>(m_plState, arg5);
			push_obj<Arg6>(m_plState, arg6);
			push_obj<Arg7>(m_plState, arg7);
			push_obj<Arg8>(m_plState, arg8);
			push_obj<Arg9>(m_plState, arg9);
			return doLuaCall<Ret>::doCall(m_plState, 10, 0, true);
		}

		lua_State *getState() const {
			return m_plState;
		}

		int getIndex() const {
			return m_iKeyIndex;
		}

		bool isNULL() const {
			return m_iKeyIndex <= 0;
		}

	private:
		lua_State *m_plState;
		int m_iKeyIndex;
		int *counter;
	};
}
#endif
