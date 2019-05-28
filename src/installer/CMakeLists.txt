install(
    PROGRAMS appimage-services-installer.sh appimage-services-uninstaller.sh
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY utils DESTINATION ${CMAKE_INSTALL_BINDIR})

