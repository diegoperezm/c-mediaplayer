cmake_minimum_required(VERSION 3.16)
project(cmediaplayer C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

if (NOT DEFINED CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif ()

message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
# cmake -DCMAKE_BUILD_TYPE=Debug
# cmake -DCMAKE_BUILD_TYPE=Release

# not working
#set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/_deps" CACHE PATH "Base directory for FetchContent downloads")

# from clay github.com/nicbarker/clay
# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
        raylib
        GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
        GIT_TAG "c1ab645ca298a2801097931d1079b10ff7eb9df8"
        GIT_PROGRESS TRUE
        GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

message(STATUS "raylib downloaded to: ${raylib_SOURCE_DIR}")

FetchContent_Declare(
        curl
        URL https://curl.se/download/curl-8.14.1.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE)


# Enable  HTTP,  no openssl/ssl
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(CURL_ENABLE_SSL OFF CACHE BOOL "" FORCE)
set(CURL_USE_OPENSSL OFF CACHE BOOL "" FORCE)
set(HTTP_ONLY ON CACHE BOOL "" FORCE)
set(CURL_USE_LIBPSL OFF CACHE BOOL "" FORCE)
set(BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(CURL_DISABLE_MANUAL ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_TESTS ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_FTP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_LDAP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_TELNET ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_DICT ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_FILE ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_TFTP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_RTSP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_GOPHER ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_IMAP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_POP3 ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_SMTP ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_SMB ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_MQTT ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_LDAPS ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_PROXY ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_COOKIES ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_ALTSVC ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_PROGRESS_METER ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_VERBOSE_STRINGS ON CACHE BOOL "" FORCE)
set(CURL_DISABLE_NTLM ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(curl)

include_directories(include)
file(GLOB SOURCES "src/*.c")

find_package(PkgConfig REQUIRED)

if (EXISTS "/Library/Frameworks/GStreamer.framework/Headers")
    include_directories(/Library/Frameworks/GStreamer.framework/Headers)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -F/Library/Frameworks -framework GStreamer")
    set(GSTREAMER_LIBRARIES "-framework GStreamer")
else ()
    message("GStreamer not found, macOs")
    pkg_check_modules(GSTREAMER gstreamer-1.0 gstreamer-base-1.0)
    if (GSTREAMER_FOUND)
        message(STATUS "GStreamer found via pkg-config")
        include_directories(${GSTREAMER_INCLUDE_DIRS})
        link_directories(${GSTREAMER_LIBRARY_DIRS})
        add_definitions(${GSTREAMER_CFLAGS_OTHER})
    else ()
        message(FATAL_ERROR "GStremer not found")
    endif ()
endif ()


set(CMAKE_C_FLAGS_DEBUG "-g -O0 -fsanitize=address,undefined")
set(CMAKE_C_FLAGS_RELEASE "-O2")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(EXECUTABLE_NAME "debug_cmediaplayer")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address,undefined")

elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(EXECUTABLE_NAME "release_cmediaplayer")
else ()
    set(EXECUTABLE_NAME "cmediaplayer")
endif ()

add_executable(${EXECUTABLE_NAME} ${SOURCES})
target_include_directories(${EXECUTABLE_NAME} PRIVATE ${curl_SOURCE_DIR}/include)
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE RAYGUI_IMPLEMENTATION)
target_link_libraries(${EXECUTABLE_NAME} raylib ${GSTREAMER_LIBRARIES} libcurl)

# Clean only build artifacts
#cmake --build . --target clean

# Rebuild without re-downloading deps
#cmake --build .

#set(CMAKE_C_FLAGS_DEBUG "-std=c99 \
#-g3 -Wall -Werror -Wconversion \
#-Wdouble-promotion -fsanitize=address,undefined \
#-Wno-unused-parameter -Wno-unused-function")

#force re-download dependencies
#rm -rf _deps
#rm -rf build
#mkdir build
#cd build
#cmake ..


