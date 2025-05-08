#include "helloWorld.hpp"

#include <print>

extern "C" {
#include "lua5.4/lua.hpp"
}

void helloWorld() {

  std::string script = "Resources/helloWorld.lua";

  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  auto check = [](lua_State *L, int r) -> bool {
    if (r != LUA_OK) {
      std::string errormsg = lua_tostring(L, -1);
      std::println("Error: {}", errormsg);
      return false;
    }
    return true;
  };

  // result: calc (10, 0, /) = Error! Division by zero.
  float num1 = 10.0;
  float num2 = 0.00;
  std::string oper = "/";

  if (check(L, luaL_dofile(L, script.c_str()))) {
    lua_getglobal(L, "Calc");
    if (lua_isfunction(L, -1)) {
      lua_pushnumber(L, num1);
      lua_pushnumber(L, num2);
      lua_pushstring(L, oper.c_str());
      if (check(L, lua_pcall(L, 3, 1, 0))) {
        auto result = lua_tostring(L, -1);
        std::println("Calc ({}, {}, {}) = {}", num1, num2, oper, result);
      }
    }
  }

  lua_close(L);
}
