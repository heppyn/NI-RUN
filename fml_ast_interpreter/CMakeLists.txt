cmake_minimum_required(VERSION 3.15)
project(fml_ast_interpreter)

set(CMAKE_CXX_STANDARD 17)
INCLUDE_DIRECTORIES(vendor/rapidjson/include)
INCLUDE_DIRECTORIES(vendor/scopeGuard)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic -Werror -g -fno-omit-frame-pointer -fsanitize=address")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

add_executable(fml_ast_interpreter src/main.cpp src/AstNodes.h src/AstParser.cpp src/AstParser.h src/visitors/ExecVisitor.cpp src/visitors/ExecVisitor.h src/Environment.cpp src/Environment.h src/AstNodes.cpp vendor/scopeGuard/scope_guard.hpp)