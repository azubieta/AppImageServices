add_library(runtimes-shared OBJECT md5.c appimagelauncher_interface.c)

target_include_directories(runtimes-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(
    runtimes-shared
    PRIVATE "INSTALL_PREFIX=\"${CMAKE_INSTALL_BINDIR}\""
)