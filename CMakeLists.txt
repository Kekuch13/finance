cmake_minimum_required(VERSION 3.26)
project(finance)

set(CMAKE_CXX_STANDARD 20)

macro(get_win_hex outvar)
    string(REGEX MATCH "^([0-9]+)\\.([0-9]+)" ${outvar} ${CMAKE_SYSTEM_VERSION})
    math(EXPR ${outvar} "(${CMAKE_MATCH_1} << 8) + ${CMAKE_MATCH_2}" OUTPUT_FORMAT HEXADECIMAL)
endmacro()

if (WIN32)
    get_win_hex(winver)
    add_compile_definitions(_WIN32_WINNT=${winver})
    message("_WIN32_WINNT=${winver}")
endif ()

add_subdirectory(Networking)

add_subdirectory(Server)
