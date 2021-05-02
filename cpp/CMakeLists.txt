cmake_minimum_required(VERSION 3.15)

set(CMAKE_LOCAL_PROJECT_NAME Platform.Hashing)
project(${CMAKE_LOCAL_PROJECT_NAME})

include(FetchContent)
FetchContent_Declare(
        crc32
        URL https://github.com/robertvazan/crc32c-hw/archive/refs/tags/v1.2.1.zip
)
FetchContent_MakeAvailable(crc32)
FetchContent_GetProperties(crc32)
if(NOT crc32_POPULATED)
    FetchContent_Populate(crc32)
    add_subdirectory(${crc32_SOURCE_DIR} ${crc32_BINARY_DIR})
endif()


add_subdirectory(${CMAKE_LOCAL_PROJECT_NAME})
target_include_directories(${CMAKE_LOCAL_PROJECT_NAME}.Library INTERFACE ${crc32_SOURCE_DIR})

get_directory_property(IS_SUBPROJECT PARENT_DIRECTORY)

macro(INCLUDE_EXTENSION EXTENSION_NAME)
    add_subdirectory(${CMAKE_LOCAL_PROJECT_NAME}.${EXTENSION_NAME})
    target_link_libraries(${CMAKE_LOCAL_PROJECT_NAME}.${EXTENSION_NAME} PUBLIC ${CMAKE_LOCAL_PROJECT_NAME}.Library)
endmacro()

function(IF_SUPPORT EXTENSION)
    message("Importing :: ${EXTENSION}" \n)
    set(UP_EXTENSION ${EXTENSION})
    string(TOUPPER ${EXTENSION} UP_EXTENSION)
    if(DEFINED LINKS_PLATFORM_${UP_EXTENSION})
        if(${LINKS_PLATFORM_${UP_EXTENSION}})
            INCLUDE_EXTENSION(${EXTENSION})
        endif()
    elseif(NOT IS_SUBPROJECT)
        INCLUDE_EXTENSION(${EXTENSION})
    endif()
endfunction()


IF_SUPPORT(Tests)
IF_SUPPORT(Benchmarks)
