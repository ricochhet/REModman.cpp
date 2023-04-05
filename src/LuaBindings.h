#ifndef REMODMAN_LUABINDINGS_INCLUDED
#define REMODMAN_LUABINDINGS_INCLUDED

#pragma once

#include <Logger.h>
#include <imgui.h>

#include <iostream>
#include <lua.hpp>

namespace LuaBindings {
    int create_lua_state();
    int logger(lua_State* L);
}

#endif  // REMODMAN_LUABINDINGS_INCLUDED