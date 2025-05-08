#pragma once

#include <string>

extern "C" {
#include <lua5.4/lua.hpp>
}

// Script Lua
auto check(lua_State *L, int r) -> bool;

/**
 * @brief Checks if two lines have an intersection point.
 *
 * This function determines whether a line, defined by the points (x0, y0) and (x1, y1),
 * intersects with another line defined by the points (x2,y2) and (x3,y3).
 *
 * @param x0 The x-coordinate of the starting point of the first segment.
 * @param y0 The y-coordinate of the starting point of the first segment.
 * @param x1 The x-coordinate of the ending point of the first segment.
 * @param y1 The y-coordinate of the ending point of the first segment.
 * @param x2 The x-coordinate of the starting point of the second segment.
 * @param y2 The y-coordinate of the starting point of the second segment.
 * @param x3 The x-coordinate of the ending point of the second segment.
 * @param y3 The y-coordinate of the ending point of the second segment.
 * @return A string indicating the result of the check. Returns whether the lines are
 * coincident, parallel or intersecting.
 *
 * Lua-C++ integration function.
 *
 */
auto intersection(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) -> std::string;
