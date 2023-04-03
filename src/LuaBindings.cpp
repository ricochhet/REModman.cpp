#include <LuaBindings.h>
#include <iostream>

int LuaBindings::create_button(lua_State *L)
{
    lua_getglobal(L, "imgui_ctx");
    ImGuiContext* ctx = (ImGuiContext*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    ImGui::SetCurrentContext(ctx);
    if (ImGui::Button("hi"))
    {
        // The button was clicked
    }

    return 0;
}

void LuaBindings::load_lua(lua_State *L)
{
    luaL_loadfilex(L, "script.lua", NULL);
    lua_pcall(L, 0, 0, 0);
}

void LuaBindings::imgui_ctx(lua_State *L, ImGuiContext *ctx)
{
    lua_pushlightuserdata(L, (void*)ctx);
    lua_setglobal(L, "imgui_ctx");
}

void LuaBindings::bind_create_button(lua_State *L)
{
    lua_pushcfunction(L, LuaBindings::create_button);
    lua_setglobal(L, "CreateButton");

    lua_getglobal(L, "CreateButton");
    lua_call(L, 0, 0);
}

/*int LuaBindings::create_lua_state(lua_State *L, ImGuiContext *ctx)
{
    luaL_loadfilex(L, "script.lua", NULL);
    lua_pcall(L, 0, 0, 0);

    lua_pushlightuserdata(L, (void*)ctx);
    lua_setglobal(L, "imgui_ctx");

    lua_pushcfunction(L, LuaBindings::create_button);
    lua_setglobal(L, "CreateButton");
    lua_getglobal(L, "CreateButton");
    lua_call(L, 0, 0);
    lua_close(L);

    return 0;
}
*/