cmake_minimum_required(VERSION 3.27)
project(sdl2_software_renderer C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(sdl2_software_renderer src/main.c)
target_link_libraries(sdl2_software_renderer ${SDL2_LIBRARIES})
