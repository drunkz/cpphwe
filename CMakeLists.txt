cmake_minimum_required(VERSION 3.15)
project(server)
set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(git)

get_git_branch(GIT_BRANCH)
get_git_hash(GIT_HASH)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options(-utf-8)
endif()

add_subdirectory(DBServer)
