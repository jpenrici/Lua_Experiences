cmake_minimum_required(VERSION 3.28)

project(HelloWorld LANGUAGES CXX)

set(PROGRAM_NAME helloWorld_lua)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
file(GLOB SOURCES src/*.cpp)
file(GLOB RESOURCE_FILES script/*.lua)

add_executable(${PROGRAM_NAME} ${SOURCES})

find_package(Lua REQUIRED)
if (Lua_FOUND)
    message(STATUS "Lua found:")
    message(STATUS "  Include Directory: ${LUA_INCLUDE_DIR}")
    message(STATUS "  Libraries: ${LUA_LIBRARIES}")
else()
    message(FATAL_ERROR, "Lua not found! Make sure Lua is installed and configured on your system.")
endif()

file(COPY ${RESOURCE_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Resources)

include_directories(${LUA_INCLUDE_DIR})

target_link_libraries(${PROGRAM_NAME} PUBLIC ${LUA_LIBRARIES})

install(TARGETS ${PROGRAM_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
