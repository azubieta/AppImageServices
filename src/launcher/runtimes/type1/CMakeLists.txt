find_package(Threads)
find_package(PkgConfig REQUIRED)
pkg_search_module(glib REQUIRED IMPORTED_TARGET glib glib-2.0)
pkg_search_module(fuse REQUIRED IMPORTED_TARGET fuse)
pkg_search_module(zlib REQUIRED IMPORTED_TARGET zlib)

add_executable(appimage-type1-runtime main.c fuseiso.c isofs.c rock.h iso_fs.h)

set_target_properties(appimage-type1-runtime PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(
    appimage-type1-runtime
    PRIVATE runtimes-shared PkgConfig::glib PkgConfig::fuse PkgConfig::zlib ${CMAKE_THREAD_LIBS_INIT}
)

target_compile_definitions(appimage-type1-runtime PUBLIC "_FILE_OFFSET_BITS=64")

install(
    TARGETS appimage-type1-runtime
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT AppImageServices
)
