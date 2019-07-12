add_library(runtimes-shared OBJECT md5.c)

target_include_directories(runtimes-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
