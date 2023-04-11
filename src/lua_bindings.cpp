#include <lua_bindings.h>

int LuaBindings::logger(lua_State* L) {
    const char* str = lua_tostring(L, 1);
    Logger::Instance().log(str, LogLevel::Warning);
    return 0;
}

int LuaBindings::create_lua_state() {
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    luaL_loadfilex(L, "script.lua", 0);

    lua_CFunction chunk = lua_tocfunction(L, -1);
    lua_pushcfunction(L, chunk);
    lua_register(L, "logger", LuaBindings::logger);
    lua_call(L, 1, 0);

    return 0;
}