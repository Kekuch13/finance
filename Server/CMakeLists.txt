cmake_minimum_required(VERSION 3.26)
project(Server)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp)

#find_package(Boost 1.81.0 REQUIRED)
#find_package(libpqxx REQUIRED)

#include_directories(${Boost_INCLUDE_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC Networking)

#target_link_libraries(${PROJECT_NAME} LINK_PUBLIC ${Boost_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} LINK_PUBLIC libpqxx::pqxx)

target_include_directories(${PROJECT_NAME} PUBLIC Networking)

target_link_libraries(${PROJECT_NAME} PUBLIC Networking)
