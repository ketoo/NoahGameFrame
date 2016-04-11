#ifndef _LUACLASS_H
#define _LUACLASS_H
#include <string.h>
#include "luacommon.h"
#include "luaObject.h"
#include "any.h"
#include "luaFunction.h"

namespace luacpp {
	template<typename T>
	void push_obj(lua_State *L, const T &obj);

	template<typename T>
	T popvalue(lua_State *L);
	class Integer64
	{
	public:
		Integer64(int64_t val) :m_val(val)
		{
			m_flag = 0XFEDC1234;
		}

		int64_t GetValue() const
		{
			return m_val;
		}

		int GetFlag() const { return m_flag; }
		static void Register2Lua(lua_State *L);
		static void setmetatable(lua_State *L)
		{
			luaL_getmetatable(L, "kenny.lualib");
			lua_pushstring(L, "int64");
			lua_gettable(L, -2);
			lua_setmetatable(L, -3);
			lua_pop(L, 1);
		}

#ifndef I64_RELA
#define I64_RELA(OP)\
	Integer64 *i64self  = (Integer64 *)lua_touserdata(L,1);\
	Integer64 *i64other = (Integer64 *)lua_touserdata(L,2);\
	lua_pushboolean(L,i64self->m_val OP i64other->m_val);\
	return 1;
#endif

		static int i64Le(lua_State *L)
		{
			I64_RELA(<= );
		}

		static int i64Lt(lua_State *L)
		{
			I64_RELA(< );
		}

		static int i64Eq(lua_State *L)
		{
			I64_RELA(== );
		}

#ifndef I64_MATH
#define I64_MATH(OP)\
	Integer64 *i64self  = (Integer64 *)lua_touserdata(L,1);\
	Integer64 *i64other = (Integer64 *)lua_touserdata(L,2);\
	Integer64 tmp(0);\
	if(!i64other)\
	{\
		tmp.m_val = (int64_t)lua_tonumber(L,2);\
		i64other = &tmp;\
	}\
	if(!i64self)\
	{\
		long num = (long)lua_tonumber(L,1);\
		size_t nbytes = sizeof(Integer64);\
		i64self = (Integer64*)lua_newuserdata(L, nbytes);\
		new(i64self)Integer64(num);\
		i64self->m_val =  i64self->m_val OP i64other->m_val;\
	}else\
	{\
		i64self->m_val = i64self->m_val OP i64other->m_val;\
		lua_pushlightuserdata(L,i64self);\
	}\
	setmetatable(L);\
	return 1;
#endif

		static int i64Mod(lua_State *L)
		{
			I64_MATH(%);
		}

		static int i64Div(lua_State *L)
		{
			I64_MATH(/ );
		}

		static int i64Mul(lua_State *L)
		{
			I64_MATH(*);
		}

		static int i64Add(lua_State *L)
		{
			I64_MATH(+);
		}

		static int i64Sub(lua_State *L)
		{
			I64_MATH(-);
		}

		static int i64toString(lua_State *L)
		{
			Integer64 *i64self = (Integer64 *)lua_touserdata(L, 1);
			luaL_argcheck(L, i64self != NULL, 1, "userdata expected");
			char temp[64];
#ifdef _VC
			_snprintf_s(temp, 512, "%ld", i64self->m_val);
#else
			snprintf(temp, 512, "%ld", i64self->m_val);
#endif
			lua_pushstring(L, temp);
			return 1;
		}

		static int i64Destroy(lua_State *L)
		{
			Integer64 *i64self = (Integer64 *)lua_touserdata(L, 1);
			luaL_argcheck(L, i64self != NULL, 1, "userdata expected");
			printf("i64Destroy\n");
			return 0;
		}

	private:
		int64_t m_val;
		int     m_flag;
	};

	static void pushI64(lua_State *L, int64_t value)
	{
		size_t nbytes = sizeof(Integer64);
		void *buf = lua_newuserdata(L, nbytes);
		new(buf)Integer64(value);
		Integer64::setmetatable(L);
	}

	enum {
		MEMBER_FUNCTION = 1,
		STATIC_FUNCTION,
		MEMBER_FIELD,
		CONSTRUCTOR,
	};

	template <typename T>
	struct memberfield
	{

		typedef void(*property_getter)(T *, lua_State*, void *(T::*));//for property get
		typedef void(*property_setter)(T *, lua_State*, void *(T::*));//for property set
		char tt;
		union {
			struct {
				property_getter getter;
				property_setter setter;
				void *(T::*property);
			};
			struct {
				void  (T::*mfunction)(void);    //member func
				lua_CFunction mlua_func;
			};
			struct {
				void(*sfunction)(void);       //static func
				lua_CFunction slua_func;
			};
			lua_CFunction constructor;
		};

		memberfield() {
			memset(this, 0, sizeof(*this));
		}

		template<typename PARENT>
		memberfield(const memberfield<PARENT> &p) {
			memcpy(this, &p, sizeof(p));
		}

	};

	template<typename T>
	class objUserData;

	template<typename T, typename property_type>
	static void GetPropertyData(T *self, lua_State *L, Int2Type<true>, void*(T::*field))
	{
		//for obj type
		push_obj<property_type*>(L, (property_type*)&(self->*field));
	}

	template<typename T, typename property_type>
	static void GetPropertyData(T *self, lua_State *L, Int2Type<false>, void*(T::*property))
	{
		push_obj<property_type>(L, *(property_type*)&(self->*property));
	}

	template<typename T, typename property_type>
	static void _GetProperty(T *self, lua_State *L, void*(T::*property), Int2Type<false>)
	{
		//for other type
		GetPropertyData<T, property_type>(self, L, Int2Type<!pointerTraits<property_type>::isPointer\
			&& IndexOf<SupportType, typename pointerTraits<property_type>::PointeeType>::value == -1>(), property);
	}

	template<typename T, typename property_type>
	static void _GetProperty(T *self, lua_State *L, void*(T::*property), Int2Type<true>)
	{
		//for luatable
		luatable *lt_ptr = (luatable*)(&(self->*property));
		push_obj<luatable>(L, *lt_ptr);
	}


	template<typename T, typename property_type>
	static void GetProperty(T *self, lua_State *L, void*(T::*property))
	{
		typedef LOKI_TYPELIST_1(luatable) lt;
		_GetProperty<T, property_type>(self, L, property, Int2Type<IndexOf<lt, property_type>::value == 0>());
	}

	template<typename T, typename property_type>
	static void SetPropertyData(T *self, lua_State *L, Int2Type<true>, void*(T::*property))
	{
		//empty function for obj type
	}

	template<typename T, typename property_type>
	static void SetPropertyData(T *self, lua_State *L, Int2Type<false>, void*(T::*property))
	{
		property_type new_value;
		new_value = *(property_type*)(&(self->*property)) = popvalue<property_type>(L);
		push_obj<property_type>(L, new_value);
	}

	template<typename CLASS_TYPE, typename property_type>
	struct Seter
	{
		Seter(CLASS_TYPE *self, lua_State *L, void*(CLASS_TYPE::*property))
		{
			SetPropertyData<CLASS_TYPE, property_type>(self, L, Int2Type<IndexOf<SupportType, \
				typename pointerTraits<property_type>::PointeeType>::value == -1>(), property);
		}
	};

	template<typename CLASS_TYPE, typename property_type>
	struct Seter<CLASS_TYPE, const property_type*>
	{
		Seter(CLASS_TYPE *self, lua_State *L, void*(CLASS_TYPE::*property)) {}
	};

	template<typename CLASS_TYPE>
	struct Seter<CLASS_TYPE, const char*>
	{
		Seter(CLASS_TYPE *self, lua_State *L, void*(CLASS_TYPE::*property)) {}
	};

	template<typename CLASS_TYPE>
	struct Seter<CLASS_TYPE, char*>
	{
		Seter(CLASS_TYPE *self, lua_State *L, void*(CLASS_TYPE::*property)) {}
	};


	void get_luatable(luatable &, lua_State *L);

	template<typename CLASS_TYPE>
	struct Seter<CLASS_TYPE, luatable>
	{
		Seter(CLASS_TYPE *self, lua_State *L, void*(CLASS_TYPE::*property))
		{
			luatable *lt_ptr = (luatable*)(&(self->*property));
			get_luatable(*lt_ptr, L);
		}
	};

	template<typename T, typename property_type>
	static void SetProperty(T *self, lua_State *L, void*(T::*property))
	{
		Seter<T, property_type> seter(self, L, property);
	}

	template<typename T>
	class luaClassWrapper
	{
		friend class objUserData<T>;

	public:

		static int luaopen_objlib(lua_State *L) {

			luaL_getmetatable(L, "kenny.lualib");

			lua_pushstring(L, luaRegisterClass<T>::GetClassName());

			luaL_newmetatable(L, luaRegisterClass<T>::GetClassName());
			lua_pushstring(L, "__index");
			lua_pushcfunction(L, &objUserData<T>::Index);
			lua_settable(L, -3);

			lua_pushstring(L, "__gc");
			lua_pushcfunction(L, &objUserData<T>::on_gc);
			lua_rawset(L, -3);

			lua_pushstring(L, "__newindex");
			lua_pushcfunction(L, &objUserData<T>::NewIndex);
			lua_rawset(L, -3);

			lua_rawset(L, 1);
			lua_pop(L, 1);
			return 1;
		}

		static std::map<std::string, memberfield<T> > &GetAllFields()
		{
			return fields;
		}

		static void InsertFields(const char *name, memberfield<T> &mf)
		{
			fields.insert(std::make_pair(std::string(name), mf));
		}

		static int InsertConstructors(int arg_count, memberfield<T> &mf)
		{
			if (constructors[arg_count].constructor == NULL)
			{
				constructors[arg_count] = mf;
				++constructor_size;
				return constructor_size;
			}
			return -1;
		}

	private:
		static std::map<std::string, memberfield<T> > fields;
		static int constructor_size;
		static memberfield<T> constructors[16];
	};


	template<typename T>
	std::map<std::string, memberfield<T> > luaClassWrapper<T>::fields;

	template<typename T>
	int luaClassWrapper<T>::constructor_size = 0;

	template<typename T>
	memberfield<T> luaClassWrapper<T>::constructors[16];

	extern int NewObj(lua_State *L, const void *ptr, const char *classname);

	template<typename T>
	class objUserData
	{
	public:


		static objUserData<T> *checkobjuserdata(lua_State *L, int index) {
			if (!luaRegisterClass<T>::isRegister())
				return NULL;
			void *ud = lua_touserdata(L, index);
			luaL_argcheck(L, ud != NULL, 1, "userdata expected");
			return (objUserData<T> *)ud;
		}

		static int NewObj(lua_State *L, const T *ptr)
		{
			return luacpp::NewObj(L, ptr, luaRegisterClass<T>::GetClassName());
		}

		static int NewIndex(lua_State *L)
		{
			objUserData<T> *self = checkobjuserdata(L, 1);
			const char *name = luaL_checkstring(L, 2);
			typename std::map<std::string, memberfield<T> >::iterator it = luaClassWrapper<T>::fields.find(std::string(name));
			if (it != luaClassWrapper<T>::fields.end())
			{
				memberfield<T> &mf = it->second;
				if (mf.tt == MEMBER_FIELD) mf.setter(self->ptr, L, mf.property);
			}
			return 0;
		}

		static int Index(lua_State *L)
		{
			objUserData<T> *obj = checkobjuserdata(L, 1);
			const char *name = luaL_checkstring(L, 2);
			typename std::map<std::string, memberfield<T> >::iterator it = luaClassWrapper<T>::fields.find(std::string(name));
			if (it != luaClassWrapper<T>::fields.end())
			{
				memberfield<T> &mf = it->second;
				if (mf.tt == MEMBER_FUNCTION)
				{
					lua_pushlightuserdata(L, &mf.mfunction);
					lua_pushcclosure(L, mf.mlua_func, 1);
					return 1;
				}
				else if (mf.tt == STATIC_FUNCTION)
				{
					lua_pushlightuserdata(L, (void*)mf.sfunction);
					lua_pushcclosure(L, mf.slua_func, 1);
					return 1;
				}
				else if (mf.tt == MEMBER_FIELD)
				{
					mf.getter(obj->ptr, L, mf.property);
					return 1;
				}
			}
			return 0;
		}


		static int Construct(lua_State *L)
		{
			int arg_size = lua_gettop(L);
			if (arg_size < 16 && luaClassWrapper<T>::constructors[arg_size].constructor)
			{
				luaClassWrapper<T>::constructors[arg_size].constructor(L);
				luaL_getmetatable(L, "kenny.lualib");
				lua_pushstring(L, luaRegisterClass<T>::GetClassName());
				lua_gettable(L, -2);
				lua_setmetatable(L, -3);
				lua_pop(L, 1);//pop mt kenny.lualib
			}
			else
			{
				char str[512];
#ifdef _VC
				_snprintf_s(str, 512, "%s: unsupport %d arguments constructor\n", luaRegisterClass<T>::GetClassName(), arg_size);
#else
				snprintf(str, 512, "%s: unsupport %d arguments constructor\n", luaRegisterClass<T>::GetClassName(), arg_size);
#endif
				luaL_error(L, str);
			}
			return 1;
		}

		static int on_gc(lua_State *L)
		{
			printf("on_gc\n");
			objUserData<T> *self = (objUserData<T> *)lua_touserdata(L, 1);
			if (self->construct_by_lua)
			{
				printf("create_by_lua delete\n");
				delete self->ptr;
			}
			return 0;
		}

	public:
		T *ptr;
		int m_flag;
		bool construct_by_lua;

	};


#ifndef _GETFUNC
#define _GETFUNC (*(__func*)lua_touserdata(L,lua_upvalueindex(1)))
#endif

	template<typename FUNC>
	class memberfunbinder {};

	template<typename Ret, typename Cla>
	class memberfunbinder<Ret(Cla::*)()>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			return doCall<Ret>(L, cla, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:
		typedef Ret(Cla::*__func)();

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)());
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Int2Type<true>)
		{
			(cla->*_GETFUNC)();
			return 0;
		}

	};

	template<typename Ret, typename Arg1, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1)>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			return doCall<Ret>(L, cla, arg1, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1);
			return 0;
		}

	};

	template<typename Ret, typename Arg1, typename Arg2, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2)>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			return doCall<Ret>(L, cla, arg1, arg2, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3)>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg2 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4)>
	{
	public:

		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg2 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg2 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4, Arg5)>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg5>::type tmp_arg5 = popvalue<typename GetReplaceType<Arg5>::type>(L);
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg4 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			Arg5 arg5 = GetRawValue<typename GetReplaceType<Arg5>::type>(tmp_arg5);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, arg5, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4, Arg5);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6)>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg6>::type tmp_arg6 = popvalue<typename GetReplaceType<Arg6>::type>(L);
			typename GetReplaceType<Arg5>::type tmp_arg5 = popvalue<typename GetReplaceType<Arg5>::type>(L);
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg4 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			Arg5 arg5 = GetRawValue<typename GetReplaceType<Arg5>::type>(tmp_arg5);
			Arg6 arg6 = GetRawValue<typename GetReplaceType<Arg6>::type>(tmp_arg6);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, arg5, arg6, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7)>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg7>::type tmp_arg7 = popvalue<typename GetReplaceType<Arg7>::type>(L);
			typename GetReplaceType<Arg6>::type tmp_arg6 = popvalue<typename GetReplaceType<Arg6>::type>(L);
			typename GetReplaceType<Arg5>::type tmp_arg5 = popvalue<typename GetReplaceType<Arg5>::type>(L);
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg4 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			Arg5 arg5 = GetRawValue<typename GetReplaceType<Arg5>::type>(tmp_arg5);
			Arg6 arg6 = GetRawValue<typename GetReplaceType<Arg6>::type>(tmp_arg6);
			Arg7 arg7 = GetRawValue<typename GetReplaceType<Arg7>::type>(tmp_arg7);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, arg5, arg6, arg7, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8)>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg8>::type tmp_arg8 = popvalue<typename GetReplaceType<Arg8>::type>(L);
			typename GetReplaceType<Arg7>::type tmp_arg7 = popvalue<typename GetReplaceType<Arg7>::type>(L);
			typename GetReplaceType<Arg6>::type tmp_arg6 = popvalue<typename GetReplaceType<Arg6>::type>(L);
			typename GetReplaceType<Arg5>::type tmp_arg5 = popvalue<typename GetReplaceType<Arg5>::type>(L);
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg4 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			Arg5 arg5 = GetRawValue<typename GetReplaceType<Arg5>::type>(tmp_arg5);
			Arg6 arg6 = GetRawValue<typename GetReplaceType<Arg6>::type>(tmp_arg6);
			Arg7 arg7 = GetRawValue<typename GetReplaceType<Arg7>::type>(tmp_arg7);
			Arg8 arg8 = GetRawValue<typename GetReplaceType<Arg8>::type>(tmp_arg8);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9)>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg9>::type tmp_arg9 = popvalue<typename GetReplaceType<Arg9>::type>(L);
			typename GetReplaceType<Arg8>::type tmp_arg8 = popvalue<typename GetReplaceType<Arg8>::type>(L);
			typename GetReplaceType<Arg7>::type tmp_arg7 = popvalue<typename GetReplaceType<Arg7>::type>(L);
			typename GetReplaceType<Arg6>::type tmp_arg6 = popvalue<typename GetReplaceType<Arg6>::type>(L);
			typename GetReplaceType<Arg5>::type tmp_arg5 = popvalue<typename GetReplaceType<Arg5>::type>(L);
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg4 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			Arg5 arg5 = GetRawValue<typename GetReplaceType<Arg5>::type>(tmp_arg5);
			Arg6 arg6 = GetRawValue<typename GetReplaceType<Arg6>::type>(tmp_arg6);
			Arg7 arg7 = GetRawValue<typename GetReplaceType<Arg7>::type>(tmp_arg7);
			Arg8 arg8 = GetRawValue<typename GetReplaceType<Arg8>::type>(tmp_arg8);
			Arg9 arg9 = GetRawValue<typename GetReplaceType<Arg9>::type>(tmp_arg9);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
			return 0;
		}
	};

	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////
	template<typename Ret, typename Cla>
	class memberfunbinder<Ret(Cla::*)() const>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			return doCall<Ret>(L, cla, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:
		typedef Ret(Cla::*__func)();

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)());
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Int2Type<true>)
		{
			(cla->*_GETFUNC)();
			return 0;
		}

	};


	template<typename Ret, typename Arg1, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1) const>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			return doCall<Ret>(L, cla, arg1, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1);
			return 0;
		}

	};

	template<typename Ret, typename Arg1, typename Arg2, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2) const>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			return doCall<Ret>(L, cla, arg1, arg2, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3) const>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg2 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4) const>
	{
	public:

		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg2 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg2 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4, Arg5) const>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg5>::type tmp_arg5 = popvalue<typename GetReplaceType<Arg5>::type>(L);
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg4 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			Arg5 arg5 = GetRawValue<typename GetReplaceType<Arg5>::type>(tmp_arg5);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, arg5, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4, Arg5);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6) const>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg6>::type tmp_arg6 = popvalue<typename GetReplaceType<Arg6>::type>(L);
			typename GetReplaceType<Arg5>::type tmp_arg5 = popvalue<typename GetReplaceType<Arg5>::type>(L);
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg4 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			Arg5 arg5 = GetRawValue<typename GetReplaceType<Arg5>::type>(tmp_arg5);
			Arg6 arg6 = GetRawValue<typename GetReplaceType<Arg6>::type>(tmp_arg6);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, arg5, arg6, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7) const>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg7>::type tmp_arg7 = popvalue<typename GetReplaceType<Arg7>::type>(L);
			typename GetReplaceType<Arg6>::type tmp_arg6 = popvalue<typename GetReplaceType<Arg6>::type>(L);
			typename GetReplaceType<Arg5>::type tmp_arg5 = popvalue<typename GetReplaceType<Arg5>::type>(L);
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg4 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			Arg5 arg5 = GetRawValue<typename GetReplaceType<Arg5>::type>(tmp_arg5);
			Arg6 arg6 = GetRawValue<typename GetReplaceType<Arg6>::type>(tmp_arg6);
			Arg7 arg7 = GetRawValue<typename GetReplaceType<Arg7>::type>(tmp_arg7);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, arg5, arg6, arg7, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8) const>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg8>::type tmp_arg8 = popvalue<typename GetReplaceType<Arg8>::type>(L);
			typename GetReplaceType<Arg7>::type tmp_arg7 = popvalue<typename GetReplaceType<Arg7>::type>(L);
			typename GetReplaceType<Arg6>::type tmp_arg6 = popvalue<typename GetReplaceType<Arg6>::type>(L);
			typename GetReplaceType<Arg5>::type tmp_arg5 = popvalue<typename GetReplaceType<Arg5>::type>(L);
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg4 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			Arg5 arg5 = GetRawValue<typename GetReplaceType<Arg5>::type>(tmp_arg5);
			Arg6 arg6 = GetRawValue<typename GetReplaceType<Arg6>::type>(tmp_arg6);
			Arg7 arg7 = GetRawValue<typename GetReplaceType<Arg7>::type>(tmp_arg7);
			Arg8 arg8 = GetRawValue<typename GetReplaceType<Arg8>::type>(tmp_arg8);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8);
			return 0;
		}
	};

	template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, typename Cla>
	class memberfunbinder<Ret(Cla::*)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9) const>
	{
	public:
		typedef Cla CLASS_TYPE;
		static int lua_cfunction(lua_State *L)
		{
			objUserData<Cla> *obj = objUserData<Cla>::checkobjuserdata(L, 1);
			Cla *cla = obj->ptr;
			typename GetReplaceType<Arg9>::type tmp_arg9 = popvalue<typename GetReplaceType<Arg9>::type>(L);
			typename GetReplaceType<Arg8>::type tmp_arg8 = popvalue<typename GetReplaceType<Arg8>::type>(L);
			typename GetReplaceType<Arg7>::type tmp_arg7 = popvalue<typename GetReplaceType<Arg7>::type>(L);
			typename GetReplaceType<Arg6>::type tmp_arg6 = popvalue<typename GetReplaceType<Arg6>::type>(L);
			typename GetReplaceType<Arg5>::type tmp_arg5 = popvalue<typename GetReplaceType<Arg5>::type>(L);
			typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
			typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
			typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
			typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
			Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
			Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
			Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
			Arg4 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
			Arg5 arg5 = GetRawValue<typename GetReplaceType<Arg5>::type>(tmp_arg5);
			Arg6 arg6 = GetRawValue<typename GetReplaceType<Arg6>::type>(tmp_arg6);
			Arg7 arg7 = GetRawValue<typename GetReplaceType<Arg7>::type>(tmp_arg7);
			Arg8 arg8 = GetRawValue<typename GetReplaceType<Arg8>::type>(tmp_arg8);
			Arg9 arg9 = GetRawValue<typename GetReplaceType<Arg9>::type>(tmp_arg9);
			return doCall<Ret>(L, cla, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, Int2Type<isVoid<Ret>::is_Void>());
		}

	private:

		typedef Ret(Cla::*__func)(Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9);

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Int2Type<false>)
		{
			push_obj<Result>(L, (cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9));
			return 1;
		}

		template<typename Result>
		static int doCall(lua_State *L, Cla *cla, Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5, Arg6 arg6, Arg7 arg7, Arg8 arg8, Arg9 arg9, Int2Type<true>)
		{
			(cla->*_GETFUNC)(arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9);
			return 0;
		}
	};
	////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////


	template<typename Parent, typename T>
	void DefParent(Int2Type<false>)
	{
		std::map<std::string, memberfield<Parent> > &parent_fields = luaClassWrapper<Parent>::GetAllFields();
		std::map<std::string, memberfield<T> > &filds = luaClassWrapper<T>::GetAllFields();
		typename std::map<std::string, memberfield<Parent> >::iterator it = parent_fields.begin();
		typename std::map<std::string, memberfield<Parent> >::iterator end = parent_fields.end();
		for (; it != end; ++it)
			filds.insert(std::make_pair(it->first, it->second));
	}

	template<typename Parent, typename T>
	void DefParent(Int2Type<true>) {}



	template<typename T>
	class class_def
	{
	private:
		class construct_function0
		{
		public:
			static int lua_cfunction(lua_State *L)
			{
				objUserData<T> *obj = (objUserData<T> *)lua_newuserdata(L, sizeof(objUserData<T>));
				obj->m_flag = 0x1234AFEC;
				obj->construct_by_lua = true;
				obj->ptr = new T;
				return 1;
			}
		};

		template<typename Arg1>
		class construct_function1
		{
		public:
			static int lua_cfunction(lua_State *L)
			{
				typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
				Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
				objUserData<T> *obj = (objUserData<T> *)lua_newuserdata(L, sizeof(objUserData<T>));
				obj->m_flag = 0x1234AFEC;
				obj->construct_by_lua = true;
				obj->ptr = new T(arg1);
				return 1;
			}
		};

		template<typename Arg1, typename Arg2>
		class construct_function2
		{
		public:
			static int lua_cfunction(lua_State *L)
			{
				typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
				Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
				typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
				Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
				objUserData<T> *obj = (objUserData<T> *)lua_newuserdata(L, sizeof(objUserData<T>));
				obj->m_flag = 0x1234AFEC;
				obj->construct_by_lua = true;
				obj->ptr = new T(arg1, arg2);
				return 1;
			}
		};

		template<typename Arg1, typename Arg2, typename Arg3>
		class construct_function3
		{
		public:
			static int lua_cfunction(lua_State *L)
			{
				typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
				Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
				typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
				Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
				typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
				Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
				objUserData<T> *obj = (objUserData<T> *)lua_newuserdata(L, sizeof(objUserData<T>));
				obj->m_flag = 0x1234AFEC;
				obj->construct_by_lua = true;
				obj->ptr = new T(arg1, arg2, arg3);
				return 1;
			}
		};

		template<typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		class construct_function4
		{
		public:
			static int lua_cfunction(lua_State *L)
			{
				typename GetReplaceType<Arg1>::type tmp_arg1 = popvalue<typename GetReplaceType<Arg1>::type>(L);
				Arg1 arg1 = GetRawValue<typename GetReplaceType<Arg1>::type>(tmp_arg1);
				typename GetReplaceType<Arg2>::type tmp_arg2 = popvalue<typename GetReplaceType<Arg2>::type>(L);
				Arg2 arg2 = GetRawValue<typename GetReplaceType<Arg2>::type>(tmp_arg2);
				typename GetReplaceType<Arg3>::type tmp_arg3 = popvalue<typename GetReplaceType<Arg3>::type>(L);
				Arg3 arg3 = GetRawValue<typename GetReplaceType<Arg3>::type>(tmp_arg3);
				typename GetReplaceType<Arg4>::type tmp_arg4 = popvalue<typename GetReplaceType<Arg4>::type>(L);
				Arg4 arg4 = GetRawValue<typename GetReplaceType<Arg4>::type>(tmp_arg4);
				objUserData<T> *obj = (objUserData<T> *)lua_newuserdata(L, sizeof(objUserData<T>));
				obj->m_flag = 0x1234AFEC;
				obj->construct_by_lua = true;
				obj->ptr = new T(arg1, arg2, arg3, arg4);
				return 1;
			}
		};
	public:
		class_def(lua_State *L) :L(L) {}

		template<typename property_type>
		class_def<T>& property(const char *name, property_type(T::*property))
		{
			memberfield<T> mf;
			mf.tt = MEMBER_FIELD;
			mf.getter = GetProperty<T, property_type>;
			mf.property = (void*(T::*))property;
			mf.setter = SetProperty<T, property_type>;
			luaClassWrapper<T>::InsertFields(name, mf);
			return *this;
		}

		//class member method
		template<typename FUNTOR>
		class_def<T>& method(const char *name, FUNTOR _func)
		{
			memberfield<T> mf;
			mf.tt = MEMBER_FUNCTION;
			mf.mfunction = (void(T::*)())_func;
			mf.mlua_func = memberfunbinder<FUNTOR>::lua_cfunction;
			luaClassWrapper<T>::InsertFields(name, mf);
			return *this;
		}

		//class static method
		template<typename Ret>
		class_def<T>& method(const char *name, Ret(*_func)())
		{
			memberfield<T> mf;
			mf.tt = STATIC_FUNCTION;
			mf.sfunction = (void(*)())_func;
			mf.slua_func = funbinder<Ret(*)()>::lua_cfunction;
			luaClassWrapper<T>::InsertFields(name, mf);
			return *this;
		}

		template<typename Ret, typename Arg1>
		class_def<T>& method(const char *name, Ret(*_func)(Arg1))
		{
			memberfield<T> mf;
			mf.tt = STATIC_FUNCTION;
			mf.sfunction = (void(*)())_func;
			mf.slua_func = funbinder<Ret(*)(Arg1)>::lua_cfunction;
			luaClassWrapper<T>::InsertFields(name, mf);
			return *this;
		}

		template<typename Ret, typename Arg1, typename Arg2>
		class_def<T>& method(const char *name, Ret(*_func)(Arg1, Arg2))
		{
			memberfield<T> mf;
			mf.tt = STATIC_FUNCTION;
			mf.sfunction = (void(*)())_func;
			mf.slua_func = funbinder<Ret(*)(Arg1, Arg2)>::lua_cfunction;
			luaClassWrapper<T>::InsertFields(name, mf);
			return *this;
		}

		template<typename Ret, typename Arg1, typename Arg2, typename Arg3>
		class_def<T>& method(const char *name, Ret(*_func)(Arg1, Arg2, Arg3))
		{
			memberfield<T> mf;
			mf.tt = STATIC_FUNCTION;
			mf.sfunction = (void(*)())_func;
			mf.slua_func = funbinder<Ret(*)(Arg1, Arg2, Arg3)>::lua_cfunction;
			luaClassWrapper<T>::InsertFields(name, mf);
			return *this;
		}


		template<typename Ret, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
		class_def<T>& method(const char *name, Ret(*_func)(Arg1, Arg2, Arg3, Arg4))
		{
			memberfield<T> mf;
			mf.tt = STATIC_FUNCTION;
			mf.sfunction = (void(*)())_func;
			mf.slua_func = funbinder<Ret(*)(Arg1, Arg2, Arg3, Arg4)>::lua_cfunction;
			luaClassWrapper<T>::InsertFields(name, mf);
			return *this;
		}

		template<typename ARG1>
		class_def<T>& constructor()
		{
			_constructor<ARG1>(Int2Type<isVoid<ARG1>::is_Void == 1>());
			return *this;
		}

		template<typename ARG1, typename ARG2>
		class_def<T>& constructor()
		{
			memberfield<T> mf;
			mf.tt = CONSTRUCTOR;
			mf.constructor = construct_function2<ARG1, ARG2>::lua_cfunction;
			_constructor(2, mf);
			return *this;
		}

		template<typename ARG1, typename ARG2, typename ARG3>
		class_def<T>& constructor()
		{
			memberfield<T> mf;
			mf.tt = CONSTRUCTOR;
			mf.constructor = construct_function3<ARG1, ARG2, ARG3>::lua_cfunction;
			_constructor(3, mf);
			return *this;
		}

		template<typename ARG1, typename ARG2, typename ARG3, typename ARG4>
		class_def<T>& constructor()
		{
			memberfield<T> mf;
			mf.tt = CONSTRUCTOR;
			mf.constructor = construct_function4<ARG1, ARG2, ARG3, ARG4>::lua_cfunction;
			_constructor(4, mf);
			return *this;
		}

	private:
		template<typename ARG1>
		void _constructor(Int2Type<true>)
		{
			memberfield<T> mf;
			mf.tt = CONSTRUCTOR;
			mf.constructor = construct_function0::lua_cfunction;
			_constructor(0, mf);
		}

		template<typename ARG1>
		void _constructor(Int2Type<false>)
		{
			memberfield<T> mf;
			mf.tt = CONSTRUCTOR;
			mf.constructor = construct_function1<ARG1>::lua_cfunction;
			_constructor(1, mf);
		}

		void _constructor(int arg_size, memberfield<T> &mf)
		{
			if (1 == luaClassWrapper<T>::InsertConstructors(arg_size, mf))
			{
				lua_getglobal(L, "_G");
				if (!lua_istable(L, -1))
				{
					lua_pop(L, 1);
					lua_newtable(L);
					lua_pushvalue(L, -1);
					lua_setglobal(L, "_G");
				}
				lua_pushstring(L, luaRegisterClass<T>::GetClassName());
				lua_pushcfunction(L, objUserData<T>::Construct);
				lua_settable(L, -3);
				lua_pop(L, 1);
			}
		}
		lua_State *L;

	};

	template<typename T, typename Parent1 = void, typename Parent2 = void, typename Parent3 = void, typename Parent4 = void>
	struct reg_cclass
	{
		static class_def<T> _reg(lua_State *L, const char *name)
		{
			luaRegisterClass<T>::SetClassName(name);
			luaClassWrapper<T>::luaopen_objlib(L);
			luaRegisterClass<T>::SetRegister();
			DefParent<Parent1, T>(Int2Type<isVoid<Parent1>::is_Void>());
			DefParent<Parent2, T>(Int2Type<isVoid<Parent2>::is_Void>());
			DefParent<Parent3, T>(Int2Type<isVoid<Parent3>::is_Void>());
			DefParent<Parent4, T>(Int2Type<isVoid<Parent4>::is_Void>());
			return class_def<T>(L);
		}
	};

}
#endif
