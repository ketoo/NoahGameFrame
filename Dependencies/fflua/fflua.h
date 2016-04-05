#ifndef _FF_LUA_H_
#define _FF_LUA_H_

//#ifndef  _WIN32
#include <stdint.h>
//#endif

#include <stdlib.h>
#include <lua.hpp>

#include <string>
using namespace std;

#include "fflua_type.h"
#include "fflua_register.h"

namespace ff
{
//! 表示void类型，由于void类型不能return，用void_ignore_t适配
template<typename T>
struct void_ignore_t;

template<typename T>
struct void_ignore_t
{
    typedef T value_t;
};

template<>
struct void_ignore_t<void>
{
    typedef cpp_void_t value_t;
};

#define  RET_V typename void_ignore_t<RET>::value_t

class fflua_t
{
    enum STACK_MIN_NUM_e
    {
        STACK_MIN_NUM = 20
    };
public:
    fflua_t():
		m_ls(NULL)
	{
		m_ls = ::luaL_newstate();
		::luaL_openlibs(m_ls);
	}
    virtual ~fflua_t()
    {
        if (m_ls)
        {
            ::lua_close(m_ls);
            m_ls = NULL;
        }
    }
    void dump_stack() const { fflua_tool_t::dump_stack(m_ls); }

    lua_State* get_lua_state()
    {
        return m_ls;
    }

    int  add_package_path(const string& str_)
    {
        string new_path = "package.path = package.path .. \"";
        if (str_.empty())
        {
            return -1;
        }

        if (str_[0] != ';')
        {
           new_path += ";";
        }

        new_path += str_;

        if (str_[str_.length() - 1] != '/')
        {
            new_path += "/";
        }

        new_path += "?.lua\" ";

        run_string(new_path);
        return 0;
    }
    int  load_file(const string& file_name_)// 
	{
		if (luaL_dofile(m_ls, file_name_.c_str()))
		{
			string err = fflua_tool_t::dump_error(m_ls, "cannot load file<%s>", file_name_.c_str());
			::lua_pop(m_ls, 1);
			throw lua_exception_t(err);
		}

		return 0;
	}
    template<typename T>
    void open_lib(T arg_);

    void run_string(const char* str_) 
	{
		if (luaL_dostring(m_ls, str_))
		{
			string err = fflua_tool_t::dump_error(m_ls, "fflua_t::run_string ::lua_pcall faled str<%s>", str_);
			::lua_pop(m_ls, 1);
			throw lua_exception_t(err);
		}
	}
    void run_string(const string& str_) 
    {
        run_string(str_.c_str());
    }



    template<typename T>
    int  get_global_variable(const string& field_name_, T& ret_);
    template<typename T>
    int  get_global_variable(const char* field_name_, T& ret_);

    template<typename T>
    int  set_global_variable(const string& field_name_, const T& value_);
    template<typename T>
    int  set_global_variable(const char* field_name_, const T& value_);

    void  register_raw_function(const char* func_name_, lua_function_t func_)
    {
        lua_checkstack(m_ls, STACK_MIN_NUM);

        lua_pushcfunction(m_ls, func_);
        lua_setglobal(m_ls, func_name_);
    }

    template<typename T>
    void  reg(T a);

    void call(const char* func_name_) 
	{
		::lua_getglobal(m_ls, func_name_);

		if (::lua_pcall(m_ls, 0, 0, 0) != 0)
		{
			string err = fflua_tool_t::dump_error(m_ls, "lua_pcall faled func_name<%s>", func_name_);
			::lua_pop(m_ls, 1);
			throw lua_exception_t(err);
		}
	}

    template<typename RET>
    RET_V call(const char* func_name_) ;

    template<typename RET, typename ARG1>
    RET_V call(const char* func_name_, const ARG1& arg1_) ;

    template<typename RET, typename ARG1, typename ARG2>
    RET_V call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_) ;

    template<typename RET, typename ARG1, typename ARG2, typename ARG3>
    RET_V call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_,
             const ARG3& arg3_) ;

    template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
    RET_V call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
             const ARG4& arg4_) ;

    template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4,
             typename ARG5>
    RET_V call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
             const ARG4& arg4_, const ARG5& arg5_) ;

    template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4,
             typename ARG5, typename ARG6>
    RET_V call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
             const ARG4& arg4_, const ARG5& arg5_, const ARG6& arg6_) ;

    template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4,
             typename ARG5, typename ARG6, typename ARG7>
    RET_V call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
             const ARG4& arg4_, const ARG5& arg5_, const ARG6& arg6_,
             const ARG7& arg7_) ;

    template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4,
             typename ARG5, typename ARG6, typename ARG7, typename ARG8>
    RET_V call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
             const ARG4& arg4_, const ARG5& arg5_, const ARG6& arg6_, const ARG7& arg7_,
             const ARG8& arg8_) ;

    template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4,
             typename ARG5, typename ARG6, typename ARG7, typename ARG8, typename ARG9>
    RET_V call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
             const ARG4& arg4_, const ARG5& arg5_, const ARG6& arg6_, const ARG7& arg7_,
             const ARG8& arg8_, const ARG9& arg9_) ;

private:
    lua_State*  m_ls;
};

template<typename T>
void fflua_t::open_lib(T arg_)
{
    arg_(m_ls);
}

template<typename T>
int  fflua_t::get_global_variable(const string& field_name_, T& ret_)
{
    return get_global_variable<T>(field_name_.c_str(), ret_);
}

template<typename T>
int  fflua_t::get_global_variable(const char* field_name_, T& ret_)
{
     int ret = 0;

     lua_getglobal(m_ls, field_name_);
     ret = lua_op_t<T>::get_ret_value(m_ls, -1, ret_);

     lua_pop(m_ls, 1);
     return ret;
}

template<typename T>
int  fflua_t::set_global_variable(const string& field_name_, const T& value_)
{
    return set_global_variable<T>(field_name_.c_str(), value_);
}

template<typename T>
int  fflua_t::set_global_variable(const char* field_name_, const T& value_)
{
    lua_op_t<T>::push_stack(m_ls, value_);
    lua_setglobal(m_ls, field_name_);
    return 0;
}

template<typename T>
void  fflua_t::reg(T a)
{
    a(this->get_lua_state());
}

//! impl for common RET
template<typename RET>
RET_V fflua_t::call(const char* func_name_) 
{
    RET_V ret = init_value_traits_t<RET_V>::value();

    lua_getglobal(m_ls, lua_string_tool_t::c_str(func_name_));

    if (lua_pcall(m_ls, 0, 1, 0) != 0)
    {
        string err = fflua_tool_t::dump_error(m_ls, "lua_pcall failed func_name<%s>", func_name_);
        lua_pop(m_ls, 1);
        throw lua_exception_t(err);
    }

    if (lua_op_t<RET_V>::get_ret_value(m_ls, -1, ret))
    {
        lua_pop(m_ls, 1);
        char buff[512];
        SPRINTF_F(buff, sizeof(buff), "callfunc [arg0] get_ret_value failed  func_name<%s>", func_name_);
        throw lua_exception_t(buff);
    }

    lua_pop(m_ls, 1);

    return ret;
}


template<typename RET, typename ARG1>
RET_V fflua_t::call(const char* func_name_, const ARG1& arg1_) 
{
    RET_V ret = init_value_traits_t<RET_V>::value();

    lua_getglobal(m_ls, lua_string_tool_t::c_str(func_name_));

    lua_op_t<ARG1>::push_stack(m_ls, arg1_);

    if (lua_pcall(m_ls, 1, 1, 0) != 0)
    {
        string err = fflua_tool_t::dump_error(m_ls, "lua_pcall failed func_name<%s>", func_name_);
        lua_pop(m_ls, 1);
        throw lua_exception_t(err);
    }

    if (lua_op_t<RET_V>::get_ret_value(m_ls, -1, ret))
    {
        lua_pop(m_ls, 1);
        char buff[512];
        SPRINTF_F(buff, sizeof(buff), "callfunc [arg1] get_ret_value failed  func_name<%s>", func_name_);
        throw lua_exception_t(buff);
    }

    lua_pop(m_ls, 1);

    return ret;
}


template<typename RET, typename ARG1, typename ARG2>
RET_V fflua_t::call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_)
                                 
{
    RET_V ret = init_value_traits_t<RET_V>::value();

    lua_getglobal(m_ls, lua_string_tool_t::c_str(func_name_));

    lua_op_t<ARG1>::push_stack(m_ls, arg1_);
    lua_op_t<ARG2>::push_stack(m_ls, arg2_);

    if (lua_pcall(m_ls, 2, 1, 0) != 0)
    {
        string err = fflua_tool_t::dump_error(m_ls, "lua_pcall failed func_name<%s>", func_name_);
        lua_pop(m_ls, 1);
        throw lua_exception_t(err);
    }

    if (lua_op_t<RET_V>::get_ret_value(m_ls, -1, ret))
    {
        lua_pop(m_ls, 1);
        char buff[512];
        SPRINTF_F(buff, sizeof(buff), "callfunc [arg2] get_ret_value failed  func_name<%s>", func_name_);
        throw lua_exception_t(buff);
    }

    lua_pop(m_ls, 1);

    return ret;
}

template<typename RET, typename ARG1, typename ARG2, typename ARG3>
RET_V fflua_t::call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_,
                                 const ARG3& arg3_) 
{
    RET_V ret = init_value_traits_t<RET_V>::value();

    lua_getglobal(m_ls, lua_string_tool_t::c_str(func_name_));

    lua_op_t<ARG1>::push_stack(m_ls, arg1_);
    lua_op_t<ARG2>::push_stack(m_ls, arg2_);
    lua_op_t<ARG3>::push_stack(m_ls, arg3_);

    if (lua_pcall(m_ls, 3, 1, 0) != 0)
    {
        string err = fflua_tool_t::dump_error(m_ls, "lua_pcall failed func_name<%s>", func_name_);
        lua_pop(m_ls, 1);
        throw lua_exception_t(err);
    }

    if (lua_op_t<RET_V>::get_ret_value(m_ls, -1, ret))
    {
        lua_pop(m_ls, 1);
        char buff[512];
        SPRINTF_F(buff, sizeof(buff), "callfunc [arg3] get_ret_value failed  func_name<%s>", func_name_);
        throw lua_exception_t(buff);
    }

    lua_pop(m_ls, 1);

    return ret;
}

template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
RET_V fflua_t::call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
                                 const ARG4& arg4_) 
{
    RET_V ret = init_value_traits_t<RET_V>::value();

    lua_getglobal(m_ls, lua_string_tool_t::c_str(func_name_));

    lua_op_t<ARG1>::push_stack(m_ls, arg1_);
    lua_op_t<ARG2>::push_stack(m_ls, arg2_);
    lua_op_t<ARG3>::push_stack(m_ls, arg3_);
    lua_op_t<ARG4>::push_stack(m_ls, arg4_);

    if (lua_pcall(m_ls, 4, 1, 0) != 0)
    {
        string err = fflua_tool_t::dump_error(m_ls, "lua_pcall failed func_name<%s>", func_name_);
        lua_pop(m_ls, 1);
        throw lua_exception_t(err);
    }

    if (lua_op_t<RET_V>::get_ret_value(m_ls, -1, ret))
    {
        lua_pop(m_ls, 1);
        char buff[512];
        SPRINTF_F(buff, sizeof(buff), "callfunc [arg4] get_ret_value failed  func_name<%s>", func_name_);
        throw lua_exception_t(buff);
    }

    lua_pop(m_ls, 1);

    return ret;
}

template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5>
RET_V fflua_t::call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
                                 const ARG4& arg4_, const ARG5& arg5_) 
{
    RET_V ret = init_value_traits_t<RET_V>::value();

    lua_getglobal(m_ls, lua_string_tool_t::c_str(func_name_));

    lua_op_t<ARG1>::push_stack(m_ls, arg1_);
    lua_op_t<ARG2>::push_stack(m_ls, arg2_);
    lua_op_t<ARG3>::push_stack(m_ls, arg3_);
    lua_op_t<ARG4>::push_stack(m_ls, arg4_);
    lua_op_t<ARG5>::push_stack(m_ls, arg5_);

    if (lua_pcall(m_ls, 5, 1, 0) != 0)
    {
        string err = fflua_tool_t::dump_error(m_ls, "lua_pcall failed func_name<%s>", func_name_);
        lua_pop(m_ls, 1);
        throw lua_exception_t(err);
    }

    if (lua_op_t<RET_V>::get_ret_value(m_ls, -1, ret))
    {
        lua_pop(m_ls, 1);
        char buff[512];
        SPRINTF_F(buff, sizeof(buff), "callfunc [arg5] get_ret_value failed  func_name<%s>", func_name_);
        throw lua_exception_t(buff);
    }

    lua_pop(m_ls, 1);

    return ret;
}


template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5, typename ARG6>
RET_V fflua_t::call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
                                 const ARG4& arg4_, const ARG5& arg5_, const ARG6& arg6_) 
{
    RET_V ret = init_value_traits_t<RET_V>::value();

    lua_getglobal(m_ls, lua_string_tool_t::c_str(func_name_));

    lua_op_t<ARG1>::push_stack(m_ls, arg1_);
    lua_op_t<ARG2>::push_stack(m_ls, arg2_);
    lua_op_t<ARG3>::push_stack(m_ls, arg3_);
    lua_op_t<ARG4>::push_stack(m_ls, arg4_);
    lua_op_t<ARG5>::push_stack(m_ls, arg5_);
    lua_op_t<ARG6>::push_stack(m_ls, arg6_);

    if (lua_pcall(m_ls, 6, 1, 0) != 0)
    {
        string err = fflua_tool_t::dump_error(m_ls, "lua_pcall failed func_name<%s>", func_name_);
        lua_pop(m_ls, 1);
        throw lua_exception_t(err);
    }

    if (lua_op_t<RET_V>::get_ret_value(m_ls, -1, ret))
    {
        lua_pop(m_ls, 1);
        char buff[512];
        SPRINTF_F(buff, sizeof(buff), "callfunc [arg6] get_ret_value failed  func_name<%s>", func_name_);
        throw lua_exception_t(buff);
    }

    lua_pop(m_ls, 1);

    return ret;
}


template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4,
                typename ARG5, typename ARG6, typename ARG7>
RET_V fflua_t::call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
                                 const ARG4& arg4_, const ARG5& arg5_, const ARG6& arg6_,
                                 const ARG7& arg7_) 
{
    RET_V ret = init_value_traits_t<RET_V>::value();

    lua_getglobal(m_ls, lua_string_tool_t::c_str(func_name_));

    lua_op_t<ARG1>::push_stack(m_ls, arg1_);
    lua_op_t<ARG2>::push_stack(m_ls, arg2_);
    lua_op_t<ARG3>::push_stack(m_ls, arg3_);
    lua_op_t<ARG4>::push_stack(m_ls, arg4_);
    lua_op_t<ARG5>::push_stack(m_ls, arg5_);
    lua_op_t<ARG6>::push_stack(m_ls, arg6_);
    lua_op_t<ARG7>::push_stack(m_ls, arg7_);

    if (lua_pcall(m_ls, 7, 1, 0) != 0)
    {
        string err = fflua_tool_t::dump_error(m_ls, "lua_pcall failed func_name<%s>", func_name_);
        lua_pop(m_ls, 1);
        throw lua_exception_t(err);
    }

    if (lua_op_t<RET_V>::get_ret_value(m_ls, -1, ret))
    {
        lua_pop(m_ls, 1);
        char buff[512];
        SPRINTF_F(buff, sizeof(buff), "callfunc [arg7] get_ret_value failed  func_name<%s>", func_name_);
        throw lua_exception_t(buff);
    }

    lua_pop(m_ls, 1);

    return ret;
}


template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4,
                typename ARG5, typename ARG6, typename ARG7, typename ARG8>
RET_V fflua_t::call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
                                 const ARG4& arg4_, const ARG5& arg5_, const ARG6& arg6_, const ARG7& arg7_,
                                 const ARG8& arg8_) 
{
    RET_V ret = init_value_traits_t<RET_V>::value();

    lua_getglobal(m_ls, lua_string_tool_t::c_str(func_name_));

    lua_op_t<ARG1>::push_stack(m_ls, arg1_);
    lua_op_t<ARG2>::push_stack(m_ls, arg2_);
    lua_op_t<ARG3>::push_stack(m_ls, arg3_);
    lua_op_t<ARG4>::push_stack(m_ls, arg4_);
    lua_op_t<ARG5>::push_stack(m_ls, arg5_);
    lua_op_t<ARG6>::push_stack(m_ls, arg6_);
    lua_op_t<ARG7>::push_stack(m_ls, arg7_);
    lua_op_t<ARG8>::push_stack(m_ls, arg8_);

    if (lua_pcall(m_ls, 8, 1, 0) != 0)
    {
        string err = fflua_tool_t::dump_error(m_ls, "lua_pcall failed func_name<%s>", func_name_);
        lua_pop(m_ls, 1);
        throw lua_exception_t(err);
    }

    if (lua_op_t<RET_V>::get_ret_value(m_ls, -1, ret))
    {
        lua_pop(m_ls, 1);
        char buff[512];
        SPRINTF_F(buff, sizeof(buff), "callfunc [arg8] get_ret_value failed  func_name<%s>", func_name_);
        throw lua_exception_t(buff);
    }

    lua_pop(m_ls, 1);

    return ret;
}


template<typename RET, typename ARG1, typename ARG2, typename ARG3, typename ARG4,
                typename ARG5, typename ARG6, typename ARG7, typename ARG8, typename ARG9>
RET_V fflua_t::call(const char* func_name_, const ARG1& arg1_, const ARG2& arg2_, const ARG3& arg3_,
                                 const ARG4& arg4_, const ARG5& arg5_, const ARG6& arg6_, const ARG7& arg7_,
                                 const ARG8& arg8_, const ARG9& arg9_) 
{
    RET_V ret = init_value_traits_t<RET_V>::value();

    lua_getglobal(m_ls, lua_string_tool_t::c_str(func_name_));

    lua_op_t<ARG1>::push_stack(m_ls, arg1_);
    lua_op_t<ARG2>::push_stack(m_ls, arg2_);
    lua_op_t<ARG3>::push_stack(m_ls, arg3_);
    lua_op_t<ARG4>::push_stack(m_ls, arg4_);
    lua_op_t<ARG5>::push_stack(m_ls, arg5_);
    lua_op_t<ARG6>::push_stack(m_ls, arg6_);
    lua_op_t<ARG7>::push_stack(m_ls, arg7_);
    lua_op_t<ARG8>::push_stack(m_ls, arg8_);
    lua_op_t<ARG9>::push_stack(m_ls, arg9_);

    if (lua_pcall(m_ls, 9, 1, 0) != 0)
    {
        string err = fflua_tool_t::dump_error(m_ls, "lua_pcall failed func_name<%s>", func_name_);
        lua_pop(m_ls, 1);
        throw lua_exception_t(err);
    }

    if (lua_op_t<RET_V>::get_ret_value(m_ls, -1, ret))
    {
        lua_pop(m_ls, 1);
        char buff[512];
        SPRINTF_F(buff, sizeof(buff), "callfunc [arg9] get_ret_value failed func_name<%s>", func_name_);
        throw lua_exception_t(buff);
    }

    lua_pop(m_ls, 1);

    return ret;
}

}
#endif
