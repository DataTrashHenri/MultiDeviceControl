cmake_minimum_required(VERSION 4.1)
project(MultiDeviceControl C)

set(CMAKE_C_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)   # don't re-check the network on every reconfigure

FetchContent_Declare(
        libuv
        GIT_REPOSITORY https://github.com/libuv/libuv.git
        GIT_TAG        v1.51.0   # or a newer release tag
)
FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.5
)
FetchContent_Declare(
        clay
        GIT_REPOSITORY https://github.com/nicbarker/clay.git
        GIT_TAG        v0.14
        GIT_SHALLOW    TRUE
        SOURCE_SUBDIR  does-not-exist   # download only, skip Clay's own CMakeLists
)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(libuv raylib clay)

add_executable(client   agent/main.c)
add_executable(server   host/main.c)
add_executable(clay     host/clay/main.c)

target_include_directories(clay PRIVATE ${clay_SOURCE_DIR})

target_link_libraries(client PRIVATE uv)   # static; use `uv` for shared
target_link_libraries(server PRIVATE uv)   # static; use `uv` for shared
target_link_libraries(clay PRIVATE raylib m)