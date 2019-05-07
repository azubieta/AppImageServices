find_package(libappimage REQUIRED)
find_package(Qt5 CONFIG REQUIRED Core DBus)
find_package(XdgUtils REQUIRED COMPONENTS DesktopEntry)

# Generate the xml interface file from DBusInterface.h
qt5_generate_dbus_interface(
    # header
    DBusInterface.h
    # interfacename
    org.appimage.Services.Launcher.xml
    # aditional qdbuscpp2xml options
    OPTIONS -A
)

# Create a launcher adaptor (header and implementation file) from the xml file
qt5_add_dbus_adaptor(
    AppImageServicesSRCS
    # xmlfile
    ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Launcher.xml
    # parentheader
    DBusInterface.h
    # parentclassname
    DBusInterface
)
set(CMAKE_AUTOMOC ON)

add_library(AppImageServicesLauncher OBJECT LauncherService.cpp ${AppImageServicesSRCS} DBusInterface.cpp)

target_include_directories(
    AppImageServicesLauncher
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(AppImageServicesLauncher
    PUBLIC Qt5::DBus libappimage
    PRIVATE Qt5::Core XdgUtils::DesktopEntry
    )

# Install D-Bus interface descriptor
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Launcher.xml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/dbus-1/interfaces
    COMPONENT AppImageServicesAPI
)
