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

add_executable(client client.c)
add_executable(server server.c)

target_link_libraries(client PRIVATE uv)   # static; use `uv` for shared
target_link_libraries(server PRIVATE uv)   # static; use `uv` for shared