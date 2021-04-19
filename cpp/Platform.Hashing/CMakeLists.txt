add_library(Platform.Hashing.Library INTERFACE)

target_include_directories(Platform.Hashing.Library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(Platform.Hashing.Library PROPERTIES CMAKE_CXX_STANDARD 20)