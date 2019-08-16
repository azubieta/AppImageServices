add_executable(install-helper install-helper.cpp)
target_link_libraries(install-helper AppImageServicesInstaller)
install(TARGETS install-helper RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_executable(uninstall-helper uninstall-helper.cpp)
target_link_libraries(uninstall-helper AppImageServicesInstaller)
install(TARGETS uninstall-helper RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

###
# Self install scripts
###
install(
        PROGRAMS appimage-services-installer.sh appimage-services-uninstaller.sh
        DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
        PROGRAMS
        utils/launcher_desktop_entries.sh
        utils/binfmt.sh
        utils/debug-header.sh
        utils/settings.sh
        DESTINATION ${CMAKE_INSTALL_BINDIR}/utils
)
