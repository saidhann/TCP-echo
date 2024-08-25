cmake_minimum_required(VERSION 3.10)
project(TCP_Server_Client)

# Set the C standard to C11
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Add subdirectories
add_subdirectory(server)
add_subdirectory(client)
