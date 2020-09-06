cmake_minimum_required(VERSION 3.17)
project(Razor C)

set(CMAKE_C_STANDARD 11)

add_executable(
        Razor
        src/main.c
        src/listener.c
        src/raknet/packet.c
)

target_include_directories(Razor PRIVATE include)

if(MSVC)
    target_compile_options(Razor PRIVATE /W4 /WX)
    target_sources(Razor PRIVATE src/raknet/socket_win32.c)
else()
    target_compile_options(Razor PRIVATE -Wall -Wextra -Werror -pedantic)
    target_sources(Razor PRIVATE src/raknet/socket_posix.c)
endif()