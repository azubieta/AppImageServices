find_package(PkgConfig REQUIRED)
pkg_search_module(fuse REQUIRED IMPORTED_TARGET fuse)
pkg_search_module(squashfuse REQUIRED IMPORTED_TARGET squashfuse)
pkg_search_module(zlib REQUIRED IMPORTED_TARGET zlib)
pkg_search_module(lzma REQUIRED IMPORTED_TARGET lzma)

add_executable(appimage-type2-runtime main.c md5.c elf.c utils.c)

get_target_property(LIBAPPIMAGE_INCLUDE_DIRS libappimage INTERFACE_INCLUDE_DIRECTORIES)

target_include_directories(appimage-type2-runtime PRIVATE ${LIBAPPIMAGE_INCLUDE_DIRS})
target_link_libraries(appimage-type2-runtime PRIVATE PkgConfig::squashfuse dl PkgConfig::fuse PkgConfig::lzma PkgConfig::zlib pthread)

target_compile_definitions(appimage-type2-runtime PRIVATE "GIT_COMMIT=\"NONE\"")
set_target_properties(appimage-type2-runtime PROPERTIES LINKER_LANGUAGE C)

install(
    TARGETS appimage-type2-runtime
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT AppImageServices
)
