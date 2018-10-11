//
// https://github.com/SteveKChiu/lua-intf
//
// Copyright 2014, Steve K. Chiu <steve.k.chiu@gmail.com>
//
// The MIT License (http://www.opensource.org/licenses/mit-license.php)
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

class LuaException : public std::exception
{
public:
    explicit LuaException(lua_State* L) noexcept
    {
        if (lua_gettop(L) > 0) {
            m_what = lua_tostring(L, -1);
        } else {
            m_what = "unknown error";
        }
    }

    explicit LuaException(const char* msg) noexcept
        : m_what(msg)
        {}

    explicit LuaException(const std::string& msg) noexcept
        : m_what(msg)
        {}

    const char* what() const noexcept
    {
        return m_what.c_str();
    }

    static int traceback(lua_State* L)
    {
        if (!lua_isstring(L, 1)) return 1;
        lua_getglobal(L, "debug");
        if (!lua_istable(L, -1)) {
            lua_pop(L, 1);
            return 1;
        }
        lua_getfield(L, -1, "traceback");
        if (!lua_isfunction(L, -1)) {
            lua_pop(L, 2);
            return 1;
        }
        lua_pushvalue(L, 1);    // pass error message
        lua_call(L, 1, 1);      // call debug.traceback
        return 1;
    }

private:
    std::string m_what;
};
