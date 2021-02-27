cmake_minimum_required(VERSION 3.15)
project(fml_ast_interpreter)

set(CMAKE_CXX_STANDARD 17)
INCLUDE_DIRECTORIES(./rapidjson/include)

set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -pedantic -Werror -g -fno-omit-frame-pointer -fsanitize=address")
set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

add_executable(fml_ast_interpreter main.cpp AstNodes.h AstParser.cpp AstParser.h visitors/ExecVisitor.cpp visitors/ExecVisitor.h)