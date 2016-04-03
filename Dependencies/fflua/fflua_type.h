#ifndef _FF_LUA_TYPE_H_
#define _FF_LUA_TYPE_H_


//#ifndef  _WIN32
#include <stdint.h>
#define SPRINTF_F snprintf
//#else
//typedef  long int64_t;
//typedef  unsigned long uint64_t;
//typedef  int int32_t;
//typedef  unsigned int uint32_t;
//typedef  short int16_t;
//typedef  unsigned short uint16_t;
//typedef  char int8_t;
//typedef  unsigned char uint8_t;
//#define SPRINTF_F _snprintf_s

//struct strtoll_tool_t
//{
//    static long do_strtoll(const char* s, const char*, int){
//        return atol(s);
//    }
//};
//#define strtoll strtoll_tool_t::do_strtoll
//#define strtoull (unsigned long)strtoll_tool_t::do_strtoll
//#endif

#include <stdlib.h>
#include <lua.hpp>
#include <string.h>
#include <sstream>
#include <vector>
#include <list>
#include <set>
#include <map>
using namespace std;

namespace ff
{

#define INHERIT_TABLE "inherit_table"

struct cpp_void_t{};

struct lua_string_tool_t
{
	inline static const char* c_str(const string& s_) { return s_.c_str(); }
	inline static const char* c_str(const char* s_)   { return s_; }
};

class lua_exception_t: public exception
{
public:
    explicit lua_exception_t(const char* err_):
		m_err(err_)
	{}
    explicit lua_exception_t(const string& err_):
		m_err(err_)
	{
	}
    ~lua_exception_t() throw (){}

    const char* what() const throw () { return m_err.c_str(); }
private:
    string m_err;
};

class fflua_tool_t
{
public:
    static void dump_stack(lua_State* ls_)
    {
        int i;
        int top = lua_gettop(ls_);

        for (i = 1; i <= top; i++)
        {
            int t = lua_type(ls_, i);
            switch (t)
            {
                case LUA_TSTRING:
                {
                    printf("`%s'", lua_tostring(ls_, i));
                }
                break;
                case LUA_TBOOLEAN:
                {
                    printf(lua_toboolean(ls_, i) ? "true" : "false");
                }
                break;
                case LUA_TNUMBER:
                {
                    printf("`%g`", lua_tonumber(ls_, i));
                }
                break;
                case LUA_TTABLE:
                {
                	printf("table end\n");
                     lua_pushnil(ls_);
                     while (lua_next(ls_, i) != 0) {
                       printf("	%s - %s\n",
                              lua_typename(ls_, lua_type(ls_, -2)),
                              lua_typename(ls_, lua_type(ls_, -1)));
                       lua_pop(ls_, 1);
                     }
                     printf("table end");
                }
                break;
                default:
                {
                    printf("`%s`", lua_typename(ls_, t));
                }
                break;
            }
            printf(" ");
        }
        printf("\n");
    }
    static string dump_error(lua_State* ls_, const char *fmt, ...)
    {
        string ret;
        char buff[1024];

        va_list argp;
        va_start(argp, fmt);
#ifndef _WIN32
        vsnprintf(buff, sizeof(buff), fmt, argp);
#else
        vsnprintf_s(buff, sizeof(buff), sizeof(buff), fmt, argp);
#endif
        va_end(argp);

        ret = buff;
        SPRINTF_F(buff, sizeof(buff), " tracback:%s", lua_tostring(ls_, -1));
        ret += buff;

        return ret;
    }
};

typedef int (*lua_function_t) (lua_State *L);

class lua_nil_t{};

template<typename T>
struct userdata_for_object_t
{
    userdata_for_object_t(T* p_ = NULL): obj(p_){}
    T* obj;
};


template<typename T>
struct lua_type_info_t
{
	static void set_name(const string& name_, string inherit_name_ = "")
	{
		size_t n = name_.length() > sizeof(name) - 1? sizeof(name) - 1: name_.length();
#ifndef _WIN32
		::strncpy(name, name_.c_str(), n);
#else
        ::strncpy_s(name, name_.c_str(), n);
#endif
		if (false == inherit_name_.empty())
		{
			n = inherit_name_.length() > sizeof(inherit_name) - 1? sizeof(inherit_name) - 1: inherit_name_.length();
#ifndef _WIN32
			::strncpy(inherit_name, inherit_name_.c_str(), n);
#else
            ::strncpy_s(inherit_name, inherit_name_.c_str(), n);
#endif
		}
	}
	inline static const char* get_name()
	{
		return name;
	}
	inline static const char* get_inherit_name()
	{
		return inherit_name;
	}
	inline static bool is_registed()
	{
		return name[0] != '\0';
	}
	inline static bool is_inherit()
	{
		return inherit_name[0] != '\0';
	}
	static char name[128];
	static char inherit_name[128];
};
template<typename T>
char lua_type_info_t<T>::name[128] = {0};
template<typename T>
char lua_type_info_t<T>::inherit_name[128] = {0};


template<typename ARG_TYPE>
struct reference_traits_t;

template<typename ARG_TYPE>
struct reference_traits_t
{
    typedef ARG_TYPE arg_type_t;
};

template<>
struct reference_traits_t<const string&>
{
    typedef string arg_type_t;
};

template<>
struct reference_traits_t<string&>
{
    typedef string arg_type_t;
};

template<typename T>
struct reference_traits_t<const T*>
{
    typedef T* arg_type_t;
};
template<typename T>
struct reference_traits_t<const T&>
{
    typedef T arg_type_t;
};

template<>
struct reference_traits_t<const char*>
{
    typedef char* arg_type_t;
};

template <typename T>
struct init_value_traits_t;

template <typename T>
struct init_value_traits_t
{
    inline static T value(){ return T(); }
};

template <typename T>
struct init_value_traits_t<const T*>
{
    inline static T* value(){ return NULL; }
};

template <typename T>
struct init_value_traits_t<const T&>
{
    inline static T value(){ return T(); }
};

template <>
struct init_value_traits_t<string>
{
    inline static const char* value(){ return ""; }
};

template <>
struct init_value_traits_t<const string&>
{
    inline static const char* value(){ return ""; }
};

template<typename T>
struct lua_op_t
{
    static void push_stack(lua_State* ls_, const char* arg_)
    {
        lua_pushstring(ls_, arg_);
    }
    /*
    static int lua_to_value(lua_State* ls_, int pos_, char*& param_)
    {
        const char* str = luaL_checkstring(ls_, pos_);
        param_ = (char*)str;
        return 0;
    }*/
};


template<>
struct lua_op_t<const char*>
{
    static void push_stack(lua_State* ls_, const char* arg_)
    {
        lua_pushstring(ls_, arg_);
    }
    static int lua_to_value(lua_State* ls_, int pos_, char*& param_)
    {
        const char* str = luaL_checkstring(ls_, pos_);
        param_ = (char*)str;
        return 0;
    }
};
template<>
struct lua_op_t<char*>
{
    static void push_stack(lua_State* ls_, const char* arg_)
    {
        lua_pushstring(ls_, arg_);
    }
    static int lua_to_value(lua_State* ls_, int pos_, char*& param_)
    {
        const char* str = luaL_checkstring(ls_, pos_);
        param_ = (char*)str;
        return 0;
    }
};

template<>
struct lua_op_t<lua_nil_t>
{
    static void push_stack(lua_State* ls_, const lua_nil_t& arg_)
    {
        lua_pushnil (ls_);
    }

};

template<>
struct lua_op_t<cpp_void_t>
{
    static int get_ret_value(lua_State* ls_, int pos_, cpp_void_t& param_)
    {
        return 0;
    }
};

template<>
struct lua_op_t<int64_t>
{
    static void push_stack(lua_State* ls_, int64_t arg_)
    {
        stringstream ss;
        ss << arg_;
        string str = ss.str();
        lua_pushlstring(ls_, str.c_str(), str.length());
    }

    static int get_ret_value(lua_State* ls_, int pos_, int64_t& param_)
    {
        if (!lua_isstring(ls_, pos_))
        {
            return -1;
        }

        size_t len  = 0;
        const char* src = lua_tolstring(ls_, pos_, &len);
        param_ = (int64_t)strtoll(src, NULL, 10);
        return 0;
    }

    static int lua_to_value(lua_State* ls_, int pos_, int64_t& param_)
    {
        size_t len = 0;
        const char* str = luaL_checklstring(ls_, pos_, &len);
        param_ = (int64_t)strtoll(str, NULL, 10);
        return 0;
    }
};

template<> struct lua_op_t<uint64_t>
{
    static void push_stack(lua_State* ls_, uint64_t arg_)
    {
    	stringstream ss;
		ss << arg_;
		string str = ss.str();
		lua_pushlstring(ls_, str.c_str(), str.length());
    }

    static int get_ret_value(lua_State* ls_, int pos_, uint64_t& param_)
    {
        if (!lua_isstring(ls_, pos_))
        {
            return -1;
        }

        size_t len  = 0;
        const char* src = lua_tolstring(ls_, pos_, &len);
        param_ = (uint64_t)strtoull(src, NULL, 10);
        return 0;
    }

    static int lua_to_value(lua_State* ls_, int pos_, uint64_t& param_)
    {
        size_t len = 0;
        const char* str = luaL_checklstring(ls_, pos_, &len);
        param_ = (uint64_t)strtoull(str, NULL, 10);
        return 0;
    }
};

template<> struct lua_op_t<int8_t>
{

	static void push_stack(lua_State* ls_, int8_t arg_)
	{
		lua_pushnumber(ls_, (lua_Number)arg_);
	}
	static int get_ret_value(lua_State* ls_, int pos_, int8_t& param_)
	{
		if (!lua_isnumber(ls_, pos_))
		{
			return -1;
		}
		param_ = (int8_t)lua_tonumber(ls_, pos_);
		return 0;
	}
	static int lua_to_value(lua_State* ls_, int pos_, int8_t& param_)
	{
		param_ = (int8_t)luaL_checknumber(ls_, pos_);
		return 0;
	}
};

template<>
struct lua_op_t<uint8_t>
{
	static void push_stack(lua_State* ls_, uint8_t arg_)
	{
		lua_pushnumber(ls_, (lua_Number)arg_);
	}
	static int get_ret_value(lua_State* ls_, int pos_, uint8_t& param_)
	{
		if (!lua_isnumber(ls_, pos_))
		{
			return -1;
		}
		param_ = (uint8_t)lua_tonumber(ls_, pos_);
		return 0;
	}
	static int lua_to_value(lua_State* ls_, int pos_, uint8_t& param_)
	{
		param_ = (uint8_t)luaL_checknumber(ls_, pos_);
		return 0;
	}
};
template<> struct lua_op_t<int16_t>
{
	static void push_stack(lua_State* ls_, int16_t arg_)
	{
		lua_pushnumber(ls_, (lua_Number)arg_);
	}
	static int get_ret_value(lua_State* ls_, int pos_, int16_t& param_)
	{
		if (!lua_isnumber(ls_, pos_))
		{
			return -1;
		}
		param_ = (int16_t)lua_tonumber(ls_, pos_);
		return 0;
	}
	static int lua_to_value(lua_State* ls_, int pos_, int16_t& param_)
	{
		param_ = (int16_t)luaL_checknumber(ls_, pos_);
		return 0;
	}
};
template<> struct lua_op_t<uint16_t>
{

	static void push_stack(lua_State* ls_, uint16_t arg_)
	{
		lua_pushnumber(ls_, (lua_Number)arg_);
	}
	static int get_ret_value(lua_State* ls_, int pos_, uint16_t& param_)
	{
		if (!lua_isnumber(ls_, pos_))
		{
			return -1;
		}
		param_ = (uint16_t)lua_tonumber(ls_, pos_);
		return 0;
	}
	static int lua_to_value(lua_State* ls_, int pos_, uint16_t& param_)
	{
		param_ = (uint16_t)luaL_checknumber(ls_, pos_);
		return 0;
	}
};
template<> struct lua_op_t<int32_t>
{
	static void push_stack(lua_State* ls_, int32_t arg_)
	{
		lua_pushnumber(ls_, (lua_Number)arg_);
	}
	static int get_ret_value(lua_State* ls_, int pos_, int32_t& param_)
	{
		if (!lua_isnumber(ls_, pos_))
		{
			return -1;
		}
		param_ = (int32_t)lua_tonumber(ls_, pos_);
		return 0;
	}
	static int lua_to_value(lua_State* ls_, int pos_, int32_t& param_)
	{
		param_ = (int32_t)luaL_checknumber(ls_, pos_);
		return 0;
	}
};
template<> struct lua_op_t<uint32_t>
{

	static void push_stack(lua_State* ls_, uint32_t arg_)
	{
		lua_pushnumber(ls_, (lua_Number)arg_);
	}
	static int get_ret_value(lua_State* ls_, int pos_, uint32_t& param_)
	{
		if (!lua_isnumber(ls_, pos_))
		{
			return -1;
		}
		param_ = (uint32_t)lua_tonumber(ls_, pos_);
		return 0;
	}
	static int lua_to_value(lua_State* ls_, int pos_, uint32_t& param_)
	{
		param_ = (uint32_t)luaL_checknumber(ls_, pos_);
		return 0;
	}
};

template<>
struct lua_op_t<bool>
{
    static void push_stack(lua_State* ls_, bool arg_)
    {
        lua_pushboolean(ls_, arg_);
    }

    static int get_ret_value(lua_State* ls_, int pos_, bool& param_)
    {
    	//! nil 自动转换为false
    	if (lua_isnil(ls_, pos_))
    	{
    		param_ = false;
    		return 0;
    	}
        if (!lua_isboolean(ls_, pos_))
        {
            return -1;
        }

        param_ = (0 != lua_toboolean(ls_, pos_));
        return 0;
    }
    static int lua_to_value(lua_State* ls_, int pos_, bool& param_)
    {
		luaL_checktype(ls_, pos_,  LUA_TBOOLEAN);
        param_ = (0 != lua_toboolean(ls_, pos_));
        return 0;
    }
};

template<>
struct lua_op_t<string>
{

    static void push_stack(lua_State* ls_, const string& arg_)
    {
        lua_pushlstring(ls_, arg_.c_str(), arg_.length());
    }

    static int get_ret_value(lua_State* ls_, int pos_, string& param_)
    {
        if (!lua_isstring(ls_, pos_))
        {
            return -1;
        }

        lua_pushvalue(ls_, pos_);
        size_t len  = 0;
        const char* src = lua_tolstring(ls_, -1, &len);
        param_.assign(src, len);
        lua_pop(ls_, 1);

        return 0;
    }
    static int lua_to_value(lua_State* ls_, int pos_, string& param_)
    {
        size_t len = 0;
        const char* str = luaL_checklstring(ls_, pos_, &len);
        param_.assign(str, len);
        return 0;
    }
};

template<>
struct lua_op_t<const string&>
{
    static void push_stack(lua_State* ls_, const string& arg_)
    {
        lua_pushlstring(ls_, arg_.c_str(), arg_.length());
    }

    static int get_ret_value(lua_State* ls_, int pos_, string& param_)
    {
        if (!lua_isstring(ls_, pos_))
        {
            return -1;
        }

        lua_pushvalue(ls_, pos_);
        size_t len  = 0;
        const char* src = lua_tolstring(ls_, -1, &len);
        param_.assign(src, len);
        lua_pop(ls_, 1);

        return 0;
    }
    static int lua_to_value(lua_State* ls_, int pos_, string& param_)
    {
        size_t len = 0;
        const char* str = luaL_checklstring(ls_, pos_, &len);
        param_.assign(str, len);
        return 0;
    }
};
template<> struct lua_op_t<float>
{
	static void push_stack(lua_State* ls_, float arg_)
	{
		lua_pushnumber(ls_, (lua_Number)arg_);
	}
	static int get_ret_value(lua_State* ls_, int pos_, float& param_)
	{
		if (!lua_isnumber(ls_, pos_))
		{
			return -1;
		}
		param_ = (float)lua_tonumber(ls_, pos_);
		return 0;
	}
	static int lua_to_value(lua_State* ls_, int pos_, float& param_)
	{
		param_ = (float)luaL_checknumber(ls_, pos_);
		return 0;
	}
};
template<> struct lua_op_t<double>
{
	static void push_stack(lua_State* ls_, double arg_)
	{
		lua_pushnumber(ls_, (lua_Number)arg_);
	}
	static int get_ret_value(lua_State* ls_, int pos_, double& param_)
	{
		if (!lua_isnumber(ls_, pos_))
		{
			return -1;
		}
		param_ = (double)lua_tonumber(ls_, pos_);
		return 0;
	}
	static int lua_to_value(lua_State* ls_, int pos_, double& param_)
	{
		param_ = (double)luaL_checknumber(ls_, pos_);
		return 0;
	}
};
/*template<> struct lua_op_t<long>
{

	static void push_stack(lua_State* ls_, long arg_)
	{
		lua_pushnumber(ls_, (lua_Number)arg_);
	}
	static int get_ret_value(lua_State* ls_, int pos_, long& param_)
	{
		if (!lua_isnumber(ls_, pos_))
		{
			return -1;
		}
		param_ = (long)lua_tonumber(ls_, pos_);
		return 0;
	}
	static int lua_to_value(lua_State* ls_, int pos_, long& param_)
	{
		param_ = (long)luaL_checknumber(ls_, pos_);
		return 0;
	}
};*/
template<>
struct lua_op_t<void*>
{
    static void push_stack(lua_State* ls_, void* arg_)
    {
        lua_pushlightuserdata(ls_, arg_);
    }

    static int get_ret_value(lua_State* ls_, int pos_, void* & param_)
    {
        if (!lua_isuserdata(ls_, pos_))
        {
            char buff[128];
            SPRINTF_F(buff, sizeof(buff), "userdata param expected, but type<%s> provided",
                                         lua_typename(ls_, lua_type(ls_, pos_)));
            printf("%s\n", buff);
            return -1;
        }

        param_ = lua_touserdata(ls_, pos_);
        return 0;
    }

    static int lua_to_value(lua_State* ls_, int pos_, void*& param_)
    {
        if (!lua_isuserdata(ls_, pos_))
        {
            luaL_argerror (ls_, 1, "userdata param expected");
            return -1;
        }
        param_ = lua_touserdata(ls_, pos_);
        return 0;
    }
};

template<typename T>
struct lua_op_t<T*>
{
    static void push_stack(lua_State* ls_, T* arg_)
    {
        void* ptr = lua_newuserdata(ls_, sizeof(userdata_for_object_t<T>));
        new (ptr) userdata_for_object_t<T>(arg_);

        luaL_getmetatable(ls_, lua_type_info_t<T>::get_name());
        lua_setmetatable(ls_, -2);
    }

    static int get_ret_value(lua_State* ls_, int pos_, T* & param_)
    {
        if (false == lua_type_info_t<T>::is_registed())
        {
            luaL_argerror(ls_, pos_, "type not supported");
        }

        void *arg_data = lua_touserdata(ls_, pos_);

        if (NULL == arg_data)
        {
            printf("expect<%s> but <%s> NULL\n", lua_type_info_t<T>::get_name(), lua_typename(ls_, lua_type(ls_, pos_)));
            return -1;
        }

        if (0 == lua_getmetatable(ls_, pos_))
        {
            return -1;
        }

        luaL_getmetatable(ls_, lua_type_info_t<T>::get_name());
        if (0 == lua_rawequal(ls_, -1, -2))
        {
        	lua_getfield(ls_, -2, INHERIT_TABLE);
        	if (0 == lua_rawequal(ls_, -1, -2))
        	{
        		printf("expect<%s> but <%s> not equal\n", lua_type_info_t<T>::get_name(), lua_typename(ls_, lua_type(ls_, pos_)));
				lua_pop(ls_, 3);
				return -1;
        	}
            lua_pop(ls_, 3);
        }
        else
        {
        	lua_pop(ls_, 2);
        }
        T* ret_ptr = ((userdata_for_object_t<T>*)arg_data)->obj;
        if (NULL == ret_ptr)
        {
            return -1;
        }

        param_ = ret_ptr;
        return 0;
    }

    static int lua_to_value(lua_State* ls_, int pos_, T*& param_)
    {
        if (false == lua_type_info_t<T>::is_registed())
        {
            luaL_argerror(ls_, pos_, "type not supported");
        }
        void *arg_data = lua_touserdata(ls_, pos_);

        if (NULL == arg_data || 0 == lua_getmetatable(ls_, pos_))
		{
			char buff[128];
			SPRINTF_F(buff, sizeof(buff), "`%s` arg1 connot be null",
										 lua_type_info_t<T>::get_name());
			luaL_argerror(ls_, pos_, buff);
		}

		luaL_getmetatable(ls_, lua_type_info_t<T>::get_name());
		if (0 == lua_rawequal(ls_, -1, -2))
		{
			lua_getfield(ls_, -2, INHERIT_TABLE);
			if (0 == lua_rawequal(ls_, -1, -2))
			{
				lua_pop(ls_, 3);
				char buff[128];
				SPRINTF_F(buff, sizeof(buff), "`%s` arg1 type not equal",
											 lua_type_info_t<T>::get_name());
				luaL_argerror(ls_, pos_, buff);
			}
			lua_pop(ls_, 3);
		}
		else
		{
			lua_pop(ls_, 2);
		}

        T* ret_ptr = ((userdata_for_object_t<T>*)arg_data)->obj;
        if (NULL == ret_ptr)
        {
            char buff[128];
            SPRINTF_F(buff, sizeof(buff), "`%s` object ptr can't be null",
                                         lua_type_info_t<T>::get_name());
            luaL_argerror(ls_, pos_, buff);
        }

        param_ = ret_ptr;
        return 0;
    }
};

template<typename T>
struct lua_op_t<const T*>
{
    static void push_stack(lua_State* ls_, const T* arg_)
    {
        lua_op_t<T*>::push_stack(ls_, (T*)arg_);
    }

    static int get_ret_value(lua_State* ls_, int pos_, T* & param_)
    {
       return lua_op_t<T*>::get_ret_value(ls_, pos_, param_);
    }

    static int lua_to_value(lua_State* ls_, int pos_, T*& param_)
    {
        return lua_op_t<T*>::lua_to_value(ls_, pos_, param_);
    }
};

template<typename T>
struct lua_op_t<vector<T> >
{
    static void push_stack(lua_State* ls_, const vector<T>& arg_)
    {
    	lua_newtable(ls_);
    	typename vector<T>::const_iterator it = arg_.begin();
    	for (int i = 1; it != arg_.end(); ++it, ++i)
    	{
    		lua_op_t<int>::push_stack(ls_, i);
    		lua_op_t<T>::push_stack(ls_, *it);
			lua_settable(ls_, -3);
    	}
    }

    static int get_ret_value(lua_State* ls_, int pos_, vector<T>& param_)
    {
    	if (0 == lua_istable(ls_, pos_))
    	{
    		return -1;
    	}
    	lua_pushnil(ls_);
    	int real_pos = pos_;
    	if (pos_ < 0) real_pos = real_pos - 1;

		while (lua_next(ls_, real_pos) != 0)
		{
			param_.push_back(T());
			if (lua_op_t<T>::get_ret_value(ls_, -1, param_[param_.size() - 1]) < 0)
			{
				return -1;
			}
			lua_pop(ls_, 1);
		}
       return 0;
    }

    static int lua_to_value(lua_State* ls_, int pos_, vector<T>& param_)
    {
    	luaL_checktype(ls_, pos_, LUA_TTABLE);

		lua_pushnil(ls_);
    	int real_pos = pos_;
    	if (pos_ < 0) real_pos = real_pos - 1;
		while (lua_next(ls_, real_pos) != 0)
		{
			param_.push_back(T());
			if (lua_op_t<T>::lua_to_value(ls_, -1, param_[param_.size() - 1]) < 0)
			{
				luaL_argerror(ls_, pos_>0?pos_:-pos_, "convert to vector failed");
			}
			lua_pop(ls_, 1);
		}
		return 0;
    }
};

template<typename T>
struct lua_op_t<list<T> >
{
    static void push_stack(lua_State* ls_, const list<T>& arg_)
    {
    	lua_newtable(ls_);
    	typename list<T>::const_iterator it = arg_.begin();
    	for (int i = 1; it != arg_.end(); ++it, ++i)
    	{
    		lua_op_t<int>::push_stack(ls_, i);
    		lua_op_t<T>::push_stack(ls_, *it);
			lua_settable(ls_, -3);
    	}
    }

    static int get_ret_value(lua_State* ls_, int pos_, list<T>& param_)
    {
    	if (0 == lua_istable(ls_, pos_))
    	{
    		return -1;
    	}
    	lua_pushnil(ls_);
    	int real_pos = pos_;
    	if (pos_ < 0) real_pos = real_pos - 1;

		while (lua_next(ls_, real_pos) != 0)
		{
			param_.push_back(T());
			if (lua_op_t<T>::get_ret_value(ls_, -1, (param_.back())) < 0)
			{
				return -1;
			}
			lua_pop(ls_, 1);
		}
       return 0;
    }

    static int lua_to_value(lua_State* ls_, int pos_, list<T>& param_)
    {
    	luaL_checktype(ls_, pos_, LUA_TTABLE);

		lua_pushnil(ls_);
    	int real_pos = pos_;
    	if (pos_ < 0) real_pos = real_pos - 1;
		while (lua_next(ls_, real_pos) != 0)
		{
			param_.push_back(T());
			if (lua_op_t<T>::lua_to_value(ls_, -1, (param_.back())) < 0)
			{
				luaL_argerror(ls_, pos_>0?pos_:-pos_, "convert to vector failed");
			}
			lua_pop(ls_, 1);
		}
		return 0;
    }
};

template<typename T>
struct lua_op_t<set<T> >
{
    static void push_stack(lua_State* ls_, const set<T>& arg_)
    {
    	lua_newtable(ls_);
    	typename set<T>::const_iterator it = arg_.begin();
    	for (int i = 1; it != arg_.end(); ++it, ++i)
    	{
    		lua_op_t<int>::push_stack(ls_, i);
    		lua_op_t<T>::push_stack(ls_, *it);
			lua_settable(ls_, -3);
    	}
    }

    static int get_ret_value(lua_State* ls_, int pos_, set<T>& param_)
    {
    	if (0 == lua_istable(ls_, pos_))
    	{
    		return -1;
    	}
    	lua_pushnil(ls_);
    	int real_pos = pos_;
    	if (pos_ < 0) real_pos = real_pos - 1;

		while (lua_next(ls_, real_pos) != 0)
		{
			T val = init_value_traits_t<T>::value();
			if (lua_op_t<T>::get_ret_value(ls_, -1, val) < 0)
			{
				return -1;
			}
			param_.insert(val);
			lua_pop(ls_, 1);
		}
       return 0;
    }

    static int lua_to_value(lua_State* ls_, int pos_, set<T>& param_)
    {
    	luaL_checktype(ls_, pos_, LUA_TTABLE);

		lua_pushnil(ls_);
    	int real_pos = pos_;
    	if (pos_ < 0) real_pos = real_pos - 1;
		while (lua_next(ls_, real_pos) != 0)
		{
			T val = init_value_traits_t<T>::value();
			if (lua_op_t<T>::lua_to_value(ls_, -1, val) < 0)
			{
				luaL_argerror(ls_, pos_>0?pos_:-pos_, "convert to vector failed");
			}
			param_.insert(val);
			lua_pop(ls_, 1);
		}
		return 0;
    }
};
template<typename K, typename V>
struct lua_op_t<map<K, V> >
{
    static void push_stack(lua_State* ls_, const map<K, V>& arg_)
    {
    	lua_newtable(ls_);
    	typename map<K, V>::const_iterator it = arg_.begin();
    	for (; it != arg_.end(); ++it)
    	{
    		lua_op_t<K>::push_stack(ls_, it->first);
    		lua_op_t<V>::push_stack(ls_, it->second);
			lua_settable(ls_, -3);
    	}
    }

    static int get_ret_value(lua_State* ls_, int pos_, map<K, V>& param_)
    {
    	if (0 == lua_istable(ls_, pos_))
    	{
    		return -1;
    	}
    	lua_pushnil(ls_);
    	int real_pos = pos_;
    	if (pos_ < 0) real_pos = real_pos - 1;

		while (lua_next(ls_, real_pos) != 0)
		{
			K key = init_value_traits_t<K>::value();
			V val = init_value_traits_t<V>::value();

			if (lua_op_t<K>::get_ret_value(ls_, -2, key) < 0 ||
				lua_op_t<V>::get_ret_value(ls_, -1, val) < 0)
			{
				return -1;
			}
			param_.insert(make_pair(key, val));
			lua_pop(ls_, 1);
		}
       return 0;
    }

    static int lua_to_value(lua_State* ls_, int pos_, map<K, V>& param_)
    {
    	luaL_checktype(ls_, pos_, LUA_TTABLE);

		lua_pushnil(ls_);
    	int real_pos = pos_;
    	if (pos_ < 0) real_pos = real_pos - 1;
		while (lua_next(ls_, real_pos) != 0)
		{
			K key = init_value_traits_t<K>::value();
			V val = init_value_traits_t<V>::value();
			if (lua_op_t<K>::get_ret_value(ls_, -2, key) < 0 ||
				lua_op_t<V>::get_ret_value(ls_, -1, val) < 0)
			{
				luaL_argerror(ls_, pos_>0?pos_:-pos_, "convert to vector failed");
			}
			param_.insert(make_pair(key, val));
			lua_pop(ls_, 1);
		}
		return 0;
    }
};

}
#endif
