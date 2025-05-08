#include "table.hpp"

#include <print>

// Data Structure
auto Record::toStr() -> std::string {
  std::string output{"Name\t: " + Name + "\n"};
  for (size_t i = 0; i < Links.size(); i++) {
    output += "Link[" + std::to_string(i) + "]\t: " + Links.at(i) + "\n";
  }
  output += "Date\t: " + Date;
  return output;
}

// Script Lua
auto check(lua_State *L, int result) -> bool {
  if (result != LUA_OK) {
    std::string errormsg = lua_tostring(L, -1);
    std::println("Error: {}", errormsg);
    lua_pop(L, 1);
    return false;
  }
  return true;
}

auto addRecord(lua_State *L, std::string name, std::vector<std::string> links,
               std::string date) -> bool {
  // Function in Lua script
  lua_getglobal(L, "AddRecord");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 1);
    return false;
  }
  // Record Name
  lua_pushstring(L, name.c_str());
  // Record Links
  lua_newtable(L);
  for (size_t i = 0; i < links.size(); ++i) {
    lua_pushstring(L, links.at(i).c_str());
    lua_rawseti(L, -2, i + 1);
  }
  // Record Date
  lua_pushstring(L, date.c_str());
  // Call function to insert records
  if (check(L, lua_pcall(L, 3, 2, 0))) {
    // Response: bool, string
    bool status = lua_toboolean(L, -2);
    auto message = lua_tostring(L, -1);
    std::println("{}\t: {}", (status ? "[OK] " : "[FAIL] "), message);
    // Free
    lua_pop(L, 2);
    return true;
  }
  return false;
}

auto getRecord(lua_State *L, int index) -> std::string {
  // Data Structure
  Record record;
  // Function in Lua script
  lua_getglobal(L, "GetRecord");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 1);
    return "Error reading record using 'GetRecord' function!";
  }
  // Argument on the stack
  lua_pushinteger(L, index);
  // Call function
  if (check(L, lua_pcall(L, 1, 1, 0))) {
    // Read Record Table
    if (!lua_istable(L, -1)) {
      lua_pop(L, 1);
      return "Error reading table 'records'! Check the index.";
    }
    // Name
    lua_getfield(L, -1, "Name");
    record.Name = lua_tostring(L, -1);
    lua_pop(L, 1);
    // Date
    lua_getfield(L, -1, "Date");
    record.Date = lua_tostring(L, -1);
    lua_pop(L, 1);
    // Links
    lua_getfield(L, -1, "Links");
    if (!lua_istable(L, -1)) {
      lua_pop(L, 1);
      return "Error reading table 'records.Links'!";
    }
    lua_Integer len = lua_rawlen(L, -1);
    for (lua_Integer i = 1; i <= len; ++i) {
      lua_rawgeti(L, -1, i);
      record.Links.push_back(lua_tostring(L, -1));
      lua_pop(L, 1);
    }
    // Free
    lua_pop(L, 2);
  }
  return record.toStr();
}

auto summary(lua_State *L) -> std::string {
  std::string text;
  // Function in Lua script
  lua_getglobal(L, "Summary");
  if (!lua_isfunction(L, -1)) {
    lua_pop(L, 1);
    return "Error reading record using 'summary' function!";
  }
  // Call function
  if (check(L, lua_pcall(L, 0, 1, 0))) {
    // Response
    text = lua_tostring(L, -1);
    // Free
    lua_pop(L, 1);
  }
  return text;
}

// Test
auto test() -> bool {
  // Script Lua
  std::string script = "Resources/table.lua";

  // API
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);

  if (!check(L, luaL_dofile(L, script.c_str()))) {
    std::println("Error loading file {}", script);
    return false;
  }

  // Add records
  addRecord(L, "01", {"link", "link"}, "01-05-2025"); // OK
  addRecord(L, "02", {"link", "link"}, "01-01-2025"); // OK
  addRecord(L, "05", {}, "2025-05-03");               // Error 'Date'
  addRecord(L, "06", {"link"}, "05/05/2025");         // Error 'Date'
  addRecord(L, "07", {"link"}, "05-05-25");           // Error 'Date'
  addRecord(L, "08", {"link"}, "01-13-2025");         // Error 'Date'
  addRecord(L, "09", {"link"}, "31-02-2025");         // Error 'Date'

  // Get record
  int index = 1;
  std::print("Record [{}]\n{}\n", index, getRecord(L, index));

  // View sumary
  std::println("Summary: {}", summary(L));

  // Close API
  lua_close(L);

  return true;
}
