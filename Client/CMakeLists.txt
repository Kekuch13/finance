cmake_minimum_required(VERSION 3.26)
project(Client)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

find_package(Boost 1.81.0 REQUIRED)

message("boost inc: ${Boost_INCLUDE_DIR}")

include_directories(${Boost_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})