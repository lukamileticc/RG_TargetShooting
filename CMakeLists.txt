cmake_minimum_required(VERSION 3.17)
project(RG_TargetShooting)

set(CMAKE_CXX_STANDARD 17)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
add_subdirectory(glad/)
add_executable(RG_TargetShooting main.cpp)

target_link_libraries(RG_TargetShooting glad glfw OpenGL::GL
        pthread X11 Xrandr Xi dl)