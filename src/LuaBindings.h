#ifndef REMODMAN_LUABINDINGS_INCLUDED
#define REMODMAN_LUABINDINGS_INCLUDED

#pragma once

#include <imgui.h>
#include <lua.hpp>

namespace LuaBindings
{
    void imgui_ctx(lua_State *L, ImGuiContext *ctx);
    int create_button(lua_State *L);
    void load_lua(lua_State *L);
    void bind_create_button(lua_State *L);
    // int create_lua_state(lua_State *L, ImGuiContext *ctx);
}

#endif REMODMAN_LUABINDINGS_INCLUDED