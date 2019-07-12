find_package(Threads)
pkg_search_module(squashfuse2 REQUIRED IMPORTED_TARGET squashfuse)

add_executable(appimage-type2-runtime main.c elf.c utils.c)
target_link_libraries(
    appimage-type2-runtime
    PRIVATE PkgConfig::squashfuse2 runtimes-shared ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS}
)

target_compile_definitions(
    appimage-type2-runtime
    PRIVATE "GIT_COMMIT=\"NONE\""
    PRIVATE "_FILE_OFFSET_BITS=64"
)

set_target_properties(appimage-type2-runtime PROPERTIES LINKER_LANGUAGE C)

install(
    TARGETS appimage-type2-runtime
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT AppImageServices
)
