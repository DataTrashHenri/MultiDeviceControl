cmake_minimum_required(VERSION 4.1)
project(MultiDeviceControl C)

set(CMAKE_C_STANDARD 23)


include(FetchContent)
FetchContent_Declare(
        libuv
        GIT_REPOSITORY https://github.com/libuv/libuv.git
        GIT_TAG        v1.51.0   # or a newer release tag
)

FetchContent_MakeAvailable(libuv)

add_executable(MultiDeviceControl main.c)
target_link_libraries(MultiDeviceControl PRIVATE uv_a)   # static; use `uv` for shared