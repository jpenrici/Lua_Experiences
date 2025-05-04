#include "matrix.hpp"

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

auto convert(Item item) -> std::tuple<bool, std::string> {

  // Sript Lua
  std::string script = "Resources/matrix.lua";

  // API
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  // Load script
  if (!check(L, luaL_dofile(L, script.c_str()))) {
    return {false, "Error loading file " + script};
  }

  // Function in Lua script
  lua_getglobal(L, "convert");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 1);
    return {false, "Error calling 'convert' function in Lua."};
  }

  // Strings
  lua_pushstring(L, item.str.c_str());
  lua_pushstring(L, item.delimiter.c_str());

  // Rows and Columns
  lua_pushinteger(L, item.rows);
  lua_pushinteger(L, item.columns);

  // Call function convert(str, delimiter, rows, columns)
  std::string response{};
  if (check(L, lua_pcall(L, 4, 1, 0))) {
    // Response: string (Matrix2D or message)
    response = lua_tostring(L, -1);
    lua_pop(L, 1);
  }

  lua_close(L);

  return {true, response};
}

void test() {

  std::println("Test ...");

  auto view = [](Item item) {
    bool status;
    std::string output;

    std::tie(status, output) =
        convert({item.str, item.delimiter, item.rows, item.columns});
    std::println("{}", output);
  };

  std::string str = "1;2;3;4;5;6;7;8;9;10;11;12";

  view({str, ";", 2, 6});
  view({str, ";", 3, 4});
  view({str, ";", 4, 3});
  view({str, ",", 4, 3});   // Check if the delimiter is correct!
  view({str, ";", -1, -1}); // Invalid array dimension!
  view({str, "", 4, 3});    // Empty 'str' or 'delimiter' parameters!
  view({"", ";", 4, 3});    // Empty 'str' or 'delimiter' parameters!
}
