#include <LuaBindings.h>

int LuaBindings::logger(lua_State *L)
{
    const char *str = lua_tostring(L, 1);
    Logger::getInstance().log(str, LogLevel::Warning);
    return 0;
}

int LuaBindings::create_lua_state()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    lua_register(L, "logger", LuaBindings::logger);

    luaL_loadfilex(L, "script.lua", 0);
    lua_call(L, 0, 0);
    return 0;
}