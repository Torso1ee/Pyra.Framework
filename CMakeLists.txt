cmake_minimum_required(VERSION 3.27.0)


project(Pyra.Framework VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
# set(CMAKE_CXX_SCAN_FOR_MODULES ON)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

message(${INCLUDE_DIR})

add_subdirectory(thirdParty)
add_subdirectory(src/framework)
add_subdirectory(example)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)