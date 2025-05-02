#pragma once

#include <vector>
#include <string>

extern "C" {
#include <lua5.4/lua.hpp>
}

// Data Structure
struct Record {
    std::string Name = "";
    std::vector<std::string> Links{};
    std::string Date = "";

    auto toStr() -> std::string;
};

// Script Lua
auto check(lua_State *L, int result) -> bool;

auto addRecord(lua_State *L, std::string name, std::vector<std::string> links,
               std::string date) -> bool ;

auto getRecord(lua_State *L, int index) -> std::string;

auto summary(lua_State *L) -> std::string;

// Test
auto test() -> bool;
