// Copyright Daniel Wallin 2009. Use, modification and distribution is
// subject to the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef LUABIND_GET_MAIN_THREAD_090321_HPP
# define LUABIND_GET_MAIN_THREAD_090321_HPP

# include <luabind/config.hpp>

namespace luabind {

LUABIND_API lua_State* get_main_thread(lua_State* L);

} // namespace luabind

#endif // LUABIND_GET_MAIN_THREAD_090321_HPP
