cmake_minimum_required(VERSION 3.10)
project(TCP_Server_Client)

#You may have to edit it for your linux, it was working for me
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(server)
add_subdirectory(client)
