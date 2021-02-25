cmake_minimum_required(VERSION 3.15)
project(fml_ast_interpreter)

set(CMAKE_CXX_STANDARD 17)
INCLUDE_DIRECTORIES(./rapidjson/include)

add_executable(fml_ast_interpreter main.cpp)