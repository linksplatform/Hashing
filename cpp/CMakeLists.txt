cmake_minimum_required(VERSION 3.17)

set(CMAKE_LOCAL_PROJECT_NAME Platform.Hashing)
project(${CMAKE_LOCAL_PROJECT_NAME})

add_subdirectory(${CMAKE_LOCAL_PROJECT_NAME})

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
