cmake_minimum_required(VERSION 3.10)
project(RM_Vision_Modular)

# 1. 设置 C++ 标准 (推荐 C++17 以支持更多现代特性，C++14 也可以)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. 性能优化 (关键步骤)
# 如果没有指定构建类型，默认使用 Release 模式 (开启 -O3 优化)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 打印当前构建类型，方便你确认
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# 3. 寻找 OpenCV 包
find_package(OpenCV REQUIRED)

# 4. 包含头文件目录
# 使用 ${PROJECT_SOURCE_DIR} 确保指向项目根目录下的 include
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${OpenCV_INCLUDE_DIRS})

# 5. 搜集源文件
# 假设 main.cpp 在根目录，其他实现在 src 目录
file(GLOB SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# 打印找到了哪些文件 (调试用)
message(STATUS "Source files found in src/: ${SRC_FILES}")

# 6. 生成可执行文件
# 将 main.cpp 和 src 下的文件一起编译
add_executable(rm_vision main.cpp ${SRC_FILES})

# 7. 链接 OpenCV 库
target_link_libraries(rm_vision ${OpenCV_LIBS})