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
#ifndef _LUAWRAPPER_H
#define _LUAWRAPPER_H

#pragma once 
#include "luacommon.h"
#include <iostream>
#include <string>
#include <map>
#include <list>
#include "luaObject.h"
#include "luaClass.h"
#include "PopValue.h"
#include "ObjPush.h"

namespace luacpp{

class luaWrapper
{
public:
	operator lua_State* ()
	{
		return lState;
	}

	void init();
	luaWrapper(){init();}
	~luaWrapper()
	{
		if(lState)
			lua_close(lState);
		lState = NULL;
	}
	
	bool dofile(const char *filename);
	bool addPackagePath(const std::string& str_)
	{
		std::string new_path = "package.path = package.path .. \"";
		std::string tmp = str_;
		if (str_.empty())
		{
			return false;
		}

		if (str_[0] != ';')
		{
			new_path += ";";
		}

		new_path += str_;
		
		if (str_[str_.length() - 1] != '/')
		{
			new_path += "/";
			tmp += "/";
		}

		new_path += "?.lua\" ";

		run_string(new_path);
		filePath.push_back(tmp);
		return true;
	}

	void run_string(const std::string& str_)
	{
		if (luaL_dostring(lState, str_.c_str()))
		{
			std::string err("Add Lua PackagePath failed, ");
			err = err + str_;
			throw(err);
		}		
	}
private:
	std::list<std::string> filePath;
	lua_State *lState;
};
}

#endif