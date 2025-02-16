cmake_minimum_required(VERSION 3.10)
project(cmediaplayer C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)
file(GLOB SOURCES "src/*.c")

find_package(raylib REQUIRED)
add_executable(cmediaplayer ${SOURCES})

target_link_libraries(cmediaplayer raylib)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}\
    -std=c99 \
    -Wall    \
    -Werror  \
    -Wconversion \
    -Wdouble-promotion \
    -Wno-sign-conversion")



#include_directories(${RAYLIB_INCLUDE_DIR})
#set(RAYLIB_INCLUDE_DIR "/opt/homebrew/Cellar/raylib/5.5/include")
#set(RAYLIB_LIB_DIR "/opt/homebrew/Cellar/raylib/5.5/lib")
#target_link_libraries(cmediaplayer raylib m pthread dl rt X11)
#target_link_libraries(cmediaplayer "-framework OpenGL" "-framework Cocoa" "-framework IOKit")
#target_compile_definitions(cmediaplayer PRIVATE RAYGUI_IMPLEMENTATION)
#if(APPLE)
  #target_link_libraries(cmediaplayer raylib m pthread dl)
  #  target_link_libraries(cmediaplayer "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
  #endif()


