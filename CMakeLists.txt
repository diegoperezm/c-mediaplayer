cmake_minimum_required(VERSION 3.31)
project(cmediaplayer C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

if(NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
   set(CMAKE_BUILD_TYPE Release)
endif ()

message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
# cmake -DCMAKE_BUILD_TYPE=Debug
# cmake -DCMAKE_BUILD_TYPE=Release

# from clay github.com/nicbarker/clay
# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG "c1ab645ca298a2801097931d1079b10ff7eb9df8"
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

message(STATUS "raylib downloaded to: ${raylib_SOURCE_DIR}")

include_directories(include)
file(GLOB SOURCES "src/*.c")

include_directories(/Library/Frameworks/GStreamer.framework/Headers)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -F/Library/Frameworks -framework GStreamer")

set(CMAKE_C_FLAGS_DEBUG "-std=c99 -g3 -O0")
set(CMAKE_C_FLAGS_RELEASE  "-std=c99")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(EXECUTABLE_NAME "debug_cmediaplayer")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(EXECUTABLE_NAME "release_cmediaplayer")
else()
    set(EXECUTABLE_NAME "cmediaplayer")
endif()

add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE RAYGUI_IMPLEMENTATION)
target_link_libraries(${EXECUTABLE_NAME} raylib)

#set(CMAKE_C_FLAGS_DEBUG "-std=c99 \
   #-g3 -Wall -Werror -Wconversion \
   #-Wdouble-promotion -fsanitize=address,undefined \
   #-Wno-unused-parameter -Wno-unused-function")


