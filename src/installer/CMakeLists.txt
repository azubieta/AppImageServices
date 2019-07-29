###
# Installer Module
###
find_package(libappimage REQUIRED)
find_package(Qt5 CONFIG REQUIRED Core DBus)
find_package(XdgUtils REQUIRED COMPONENTS DesktopEntry)

set(AppImageServicesInstallerSRCS  Installer.cpp InstallerError.cpp)
set(CMAKE_AUTOMOC ON)

add_library(AppImageServicesInstaller OBJECT ${AppImageServicesInstallerSRCS})

target_include_directories(
        AppImageServicesInstaller
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
        AppImageServicesInstaller
        PUBLIC Qt5::DBus Qt5::Core libappimage XdgUtils::DesktopEntry AppImageServicesCommon
)

###
# Self install scripts
###
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
