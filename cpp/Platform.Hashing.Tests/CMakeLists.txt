cmake_minimum_required(VERSION 3.17)
project(Platform.Hashing.Tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

include_directories(../Platform.Hashing)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()


add_executable(Platform.Hashing.Tests Platform.Hashing.TemplateLibrary.Tests.cpp)

target_link_libraries(Platform.Hashing.Tests gtest gtest_main)
