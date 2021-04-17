cmake_minimum_required(VERSION 3.17)

set(CMAKE_LOCAL_PROJECT_NAME Platform.Hashing)
project(${CMAKE_LOCAL_PROJECT_NAME})

add_subdirectory(${CMAKE_LOCAL_PROJECT_NAME})

get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

macro(INCLUDE_TESTS TESTS_NAME)
    add_subdirectory(${TESTS_NAME}.Tests)
    target_link_libraries(${TESTS_NAME}.Tests ${TESTS_NAME}.Library)
endmacro()

if(DEFINED LINKS_PLATFORM_TESTS)
    if(${LINKS_PLATFORM_TESTS})
        INCLUDE_TESTS(${CMAKE_LOCAL_PROJECT_NAME})
    endif()
elseif(NOT IS_SUBPROJECT)
    INCLUDE_TESTS(${CMAKE_LOCAL_PROJECT_NAME})
endif()

