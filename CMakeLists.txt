cmake_minimum_required(VERSION 3.27)
project(sdl2_software_renderer C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(sdl2_software_renderer
        src/main.c
        src/display.c
        src/display.h
        src/vector.h
        src/vector.c
        src/camera.c
        src/camera.h
        src/mesh.c
        src/mesh.h
        src/triangle.c
        src/triangle.h
        src/array.c
        src/array.h
)
target_link_libraries(sdl2_software_renderer ${SDL2_LIBRARIES})

# todo - link against math library?

