cmake_minimum_required(VERSION 3.8)
project(Platform.Hashing)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_library(${PROJECT_NAME}.Library INTERFACE)
target_include_directories(${PROJECT_NAME}.Library INTERFACE ${PROJECT_NAME})
target_compile_options(${PROJECT_NAME}.Library INTERFACE -pclmul-msse4.2)

add_executable(${PROJECT_NAME}.Tests ${PROJECT_NAME}.Tests/AllTests.cpp)
target_link_libraries(${PROJECT_NAME}.Tests PRIVATE ${PROJECT_NAME}.Library)
target_link_libraries(${PROJECT_NAME}.Tests PRIVATE CONAN_PKG::gtest)
set_target_properties(${PROJECT_NAME}.Tests PROPERTIES CXX_STANDARD 20)

add_executable(${PROJECT_NAME}.Benchmarks ${PROJECT_NAME}.Tests/AllTests.cpp)
target_link_libraries(${PROJECT_NAME}.Benchmarks PRIVATE ${PROJECT_NAME}.Library)
target_link_libraries(${PROJECT_NAME}.Benchmarks PRIVATE CONAN_PKG::benchmark)
set_target_properties(${PROJECT_NAME}.Benchmarks PROPERTIES CXX_STANDARD 20)
