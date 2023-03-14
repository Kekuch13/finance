cmake_minimum_required(VERSION 3.26)
project(finance)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Networking)

add_subdirectory(Client)
add_subdirectory(Server)
