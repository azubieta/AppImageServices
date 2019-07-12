install(
    PROGRAMS appimage-services-installer.sh appimage-services-uninstaller.sh
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
    PROGRAMS
    utils/update_launchers_desktop_entries.sh
    utils/debug-header.sh
    utils/settings.sh
    DESTINATION ${CMAKE_INSTALL_BINDIR}/utils
)

