#pragma once

#include <string>
#include <tuple>

#include <lua5.4/lua.hpp>

// Auxiliary Data Structure
struct Item {
    // Text
    std::string str;
    std::string delimiter;

    // Matrix2D
    int rows{0};
    int columns{0};
};

// Script Lua
auto check(lua_State *L, int r) -> bool;

auto convert(Item item) -> std::tuple<bool, std::string>;

// Test
void test();
