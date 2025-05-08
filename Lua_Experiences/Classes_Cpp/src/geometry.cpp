#include "geometry.hpp"

#include <print>

auto check(lua_State *L, int r) -> bool {
  if (r != LUA_OK) {
    std::string errormsg = lua_tostring(L, -1);
    std::println("Error: {}", errormsg);
    lua_pop(L, 1);
    return false;
  }
  return true;
}

auto intersection(int x0, int y0, int x1, int y1, int x2, int y2, int x3,
                  int y3) -> std::string {
  // Sript Lua
  std::string script = "Resources/geometry.lua";

  // API
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  // Load script
  if (!check(L, luaL_dofile(L, script.c_str()))) {
    return {"Error loading file " + script};
  }

  // Function in Lua script
  lua_getglobal(L, "Intersection");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 1);
    return {"Error calling 'Intersection' function in Lua."};
  }

  // Entries
  lua_pushinteger(L, x0);
  lua_pushinteger(L, y0);
  lua_pushinteger(L, x1);
  lua_pushinteger(L, y1);
  lua_pushinteger(L, x2);
  lua_pushinteger(L, y2);
  lua_pushinteger(L, x3);
  lua_pushinteger(L, y3);

  // Call function Intersection(x0, y0, x1, y1, x2, y2, x3, y3)
  if (check(L, lua_pcall(L, 8, 2, 0))) {
    // Response: string, string (messages)
    auto result = lua_tostring(L, -2);
    auto point = lua_tostring(L, -1);
    std::println("{}: {}", result, point);
    lua_pop(L, 2);
  }

  lua_close(L);

  return {"Lua-C++ interaction worked!"};
}
