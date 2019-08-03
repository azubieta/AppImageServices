###
# Installer Module
###
find_package(libappimage REQUIRED)
find_package(Qt5 CONFIG REQUIRED Core DBus)
find_package(XdgUtils REQUIRED COMPONENTS DesktopEntry)

set(AppImageServicesInstallerSRCS  Installer.cpp InstallerError.cpp InstallerDBusInterface.cpp)


# Generate the xml interface file from InstallerDBusInterface.h
qt5_generate_dbus_interface(
        # header
        InstallerDBusInterface.h
        # interfacename
        org.appimage.Services.Installer.xml
        # aditional qdbuscpp2xml options
        OPTIONS -A
)

# Create a launcher adaptor (header and implementation file) from the xml file
qt5_add_dbus_adaptor(
        AppImageServicesInstallerSRCS
        # xmlfile
        ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Installer.xml
        # parentheader
        InstallerDBusInterface.h
        # parentclassname
        InstallerDBusInterface
)

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

include(Helpers.cmake)