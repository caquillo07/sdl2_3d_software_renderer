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
        src/matrix.c
        src/matrix.h
        src/light.c
        src/light.h
        src/light.h
        src/texture.c
        src/texture.h
        src/swap.c
        src/swap.h
        src/upng.c
        src/upng.h
        src/clipping.c
        src/clipping.h
)
target_link_libraries(sdl2_software_renderer ${SDL2_LIBRARIES})

# todo - link against math library?

