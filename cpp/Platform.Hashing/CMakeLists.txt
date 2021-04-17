add_library(Platform.Hashing.Library INTERFACE)

target_include_directories(Platform.Hashing.Library INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_features(Platform.Hashing.Library INTERFACE cxx_std_20)