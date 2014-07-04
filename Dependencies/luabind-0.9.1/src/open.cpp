// Copyright (c) 2003 Daniel Wallin and Arvid Norberg

// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.

#define LUABIND_BUILDING

#include <luabind/lua_include.hpp>

#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/get_main_thread.hpp>

namespace luabind {

namespace
{

  int make_property(lua_State* L)
  {
      int args = lua_gettop(L);

      if (args == 0 || args > 2)
      {
          lua_pushstring(L, "make_property() called with wrong number of arguments.");
          lua_error(L);
      }

      if (args == 1)
          lua_pushnil(L);

      lua_pushcclosure(L, &detail::property_tag, 2);
      return 1;
  }

  int main_thread_tag;

  int deprecated_super(lua_State* L)
  {
      lua_pushstring(L,
          "DEPRECATION: 'super' has been deprecated in favor of "
          "directly calling the base class __init() function. "
          "This error can be disabled by calling 'luabind::disable_super_deprecation()'."
      );
      lua_error(L);

      return 0;
  }

  int destroy_class_id_map(lua_State* L)
  {
      detail::class_id_map* m =
          (detail::class_id_map*)lua_touserdata(L, 1);
      m->~class_id_map();
      return 0;
  }

  int destroy_cast_graph(lua_State* L)
  {
      detail::cast_graph* g =
          (detail::cast_graph*)lua_touserdata(L, 1);
      g->~cast_graph();
      return 0;
  }

  int destroy_class_map(lua_State* L)
  {
      detail::class_map* m =
          (detail::class_map*)lua_touserdata(L, 1);
      m->~class_map();
      return 0;
  }

} // namespace unnamed

    LUABIND_API lua_State* get_main_thread(lua_State* L)
    {
        lua_pushlightuserdata(L, &main_thread_tag);
        lua_rawget(L, LUA_REGISTRYINDEX);
        lua_State* result = static_cast<lua_State*>(lua_touserdata(L, -1));
        lua_pop(L, 1);

        if (!result)
            throw std::runtime_error("Unable to get main thread, luabind::open() not called?");

        return result;
    }

    LUABIND_API void open(lua_State* L)
    {
        bool is_main_thread = lua_pushthread(L) == 1;
        lua_pop(L, 1);

        if (!is_main_thread)
        {
            throw std::runtime_error(
                "luabind::open() must be called with the main thread "
                "lua_State*"
            );
        }

        if (detail::class_registry::get_registry(L))
            return;

        lua_pushstring(L, "__luabind_classes");
        detail::class_registry* r = static_cast<detail::class_registry*>(
            lua_newuserdata(L, sizeof(detail::class_registry)));

        // set gc metatable
        lua_newtable(L);
        lua_pushstring(L, "__gc");
        lua_pushcclosure(
            L
          , detail::garbage_collector_s<
                detail::class_registry
            >::apply
          , 0);

        lua_settable(L, -3);
        lua_setmetatable(L, -2);

        new(r) detail::class_registry(L);
        lua_settable(L, LUA_REGISTRYINDEX);

        lua_pushstring(L, "__luabind_class_id_map");
        void* classes_storage = lua_newuserdata(L, sizeof(detail::class_id_map));
        detail::class_id_map* class_ids = new (classes_storage) detail::class_id_map;
        (void)class_ids;

        lua_newtable(L);
        lua_pushcclosure(L, &destroy_class_id_map, 0);
        lua_setfield(L, -2, "__gc");
        lua_setmetatable(L, -2);

        lua_settable(L, LUA_REGISTRYINDEX);

        lua_pushstring(L, "__luabind_cast_graph");
        void* cast_graph_storage = lua_newuserdata(
            L, sizeof(detail::cast_graph));
        detail::cast_graph* graph = new (cast_graph_storage) detail::cast_graph;
        (void)graph;

        lua_newtable(L);
        lua_pushcclosure(L, &destroy_cast_graph, 0);
        lua_setfield(L, -2, "__gc");
        lua_setmetatable(L, -2);

        lua_settable(L, LUA_REGISTRYINDEX);

        lua_pushstring(L, "__luabind_class_map");
        void* class_map_storage = lua_newuserdata(
            L, sizeof(detail::class_map));
        detail::class_map* classes = new (class_map_storage) detail::class_map;
        (void)classes;

        lua_newtable(L);
        lua_pushcclosure(L, &destroy_class_map, 0);
        lua_setfield(L, -2, "__gc");
        lua_setmetatable(L, -2);

        lua_settable(L, LUA_REGISTRYINDEX);

        // add functions (class, cast etc...)
        lua_pushstring(L, "class");
        lua_pushcclosure(L, detail::create_class::stage1, 0);
        lua_settable(L, LUA_GLOBALSINDEX);

        lua_pushstring(L, "property");
        lua_pushcclosure(L, &make_property, 0);
        lua_settable(L, LUA_GLOBALSINDEX);

        lua_pushlightuserdata(L, &main_thread_tag);
        lua_pushlightuserdata(L, L);
        lua_rawset(L, LUA_REGISTRYINDEX);

        lua_pushstring(L, "super");
        lua_pushcclosure(L, &deprecated_super, 0);
        lua_settable(L, LUA_GLOBALSINDEX);
    }

} // namespace luabind

