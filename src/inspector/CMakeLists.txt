find_package(libappimage REQUIRED)
find_package(Qt5 CONFIG REQUIRED Core DBus)
find_package(XdgUtils REQUIRED COMPONENTS DesktopEntry)

set(
    AppImageServicesInspectorSRCS
    InspectorDBusInterface.cpp
    AppImageInfoReader.cpp
    AppStreamReader.cpp
    DesktopEntryReader.cpp
)

# Generate the xml interface file from LauncherDBusInterface.h
qt5_generate_dbus_interface(
    # header
    InspectorDBusInterface.h
    # interfacename
    org.appimage.Services.Inspector.xml
    # aditional qdbuscpp2xml options
    OPTIONS -A
)

# Create a launcher adaptor (header and implementation file) from the xml file
qt5_add_dbus_adaptor(
    AppImageServicesInspectorSRCS
    # xmlfile
    ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Inspector.xml
    # parentheader
    InspectorDBusInterface.h
    # parentclassname
    InspectorDBusInterface
)
set(CMAKE_AUTOMOC ON)


add_library(AppImageServicesInspector OBJECT ${AppImageServicesInspectorSRCS})

target_include_directories(
    AppImageServicesInspector
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
    AppImageServicesInspector
    PUBLIC Qt5::DBus Qt5::Core libappimage XdgUtils::DesktopEntry AppImageServicesCommon
)

# Install D-Bus interface descriptor
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Inspector.xml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/dbus-1/interfaces
    COMPONENT AppImageServicesAPI
)
