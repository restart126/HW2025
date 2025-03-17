cmake_minimum_required(VERSION 3.27)
project(code_competition)

set(CMAKE_CXX_STANDARD 17)

# 添加 include 目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 获取 realize 目录下的所有 cpp 文件
file(GLOB REALIZE_SOURCES ${PROJECT_SOURCE_DIR}/realize/*.cpp)

# 生成可执行文件
add_executable(code_competition main.cpp ${REALIZE_SOURCES})
