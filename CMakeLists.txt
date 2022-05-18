cmake_minimum_required(VERSION 3.0.0)
project(FAB02SwitchConfig VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(FAB02SwitchConfig main.cpp graph.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_EXE_LINKER_FLAGS -static)
include(CPack)
