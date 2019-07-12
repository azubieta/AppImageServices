add_subdirectory(shared)
add_subdirectory(type1)
add_subdirectory(type2)

install(PROGRAMS runtime-wrapper.sh DESTINATION ${CMAKE_INSTALL_BINDIR})
