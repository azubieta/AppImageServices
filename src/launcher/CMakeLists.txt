find_package(libappimage REQUIRED)
find_package(Qt5 CONFIG REQUIRED Core DBus)
find_package(XdgUtils REQUIRED COMPONENTS DesktopEntry)

# Runtimes
add_subdirectory(runtimes)

set(
    AppImageServicesLauncherSRCS
    LauncherService.cpp
    LauncherDBusInterface.cpp
)

# Generate the xml interface file from LauncherDBusInterface.h
qt5_generate_dbus_interface(
    # header
    LauncherDBusInterface.h
    # interfacename
    org.appimage.Services.Launcher.xml
    # aditional qdbuscpp2xml options
    OPTIONS -A
)

# Create a launcher adaptor (header and implementation file) from the xml file
qt5_add_dbus_adaptor(
    AppImageServicesLauncherSRCS
    # xmlfile
    ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Launcher.xml
    # parentheader
    LauncherDBusInterface.h
    # parentclassname
    LauncherDBusInterface
)
set(CMAKE_AUTOMOC ON)


add_library(AppImageServicesLauncher OBJECT ${AppImageServicesLauncherSRCS})

target_include_directories(
    AppImageServicesLauncher
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
    AppImageServicesLauncher
    PUBLIC Qt5::DBus Qt5::Core libappimage XdgUtils::DesktopEntry
)

# Install D-Bus interface descriptor
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Launcher.xml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/dbus-1/interfaces
    COMPONENT AppImageServicesAPI
)
