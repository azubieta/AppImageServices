find_package(libappimage REQUIRED)
find_package(Qt5 CONFIG REQUIRED Core DBus Network)

set(
    AppImageServicesUpdaterSRCS
    UpdaterService.cpp
    UpdaterDBusInterface.cpp
    UpdaterTaskDBusInterface.cpp UpdaterTaskDBusInterface.h)

# Generate the xml interface file
qt5_generate_dbus_interface(
    # header
    UpdaterDBusInterface.h
    # interfacename
    org.appimage.Services.Updater.xml
    # aditional qdbuscpp2xml options
    OPTIONS -A
)

# Create a updater adaptor (header and implementation file) from the xml file
qt5_add_dbus_adaptor(
    AppImageServicesUpdaterSRCS
    # xmlfile
    ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Updater.xml
    # parentheader
    UpdaterDBusInterface.h
    # parentclassname
    UpdaterDBusInterface
)


# Generate the xml interface file
qt5_generate_dbus_interface(
    # header
    UpdaterTaskDBusInterface.h
    # interfacename
    org.appimage.Services.Updater.Task.xml
    # aditional qdbuscpp2xml options
    OPTIONS -A
)

# Create a updater adaptor (header and implementation file) from the xml file
qt5_add_dbus_adaptor(
    AppImageServicesUpdaterSRCS
    # xmlfile
    ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Updater.Task.xml
    # parentheader
    UpdaterTaskDBusInterface.h
    # parentclassname
    UpdaterTaskDBusInterface
)

set(CMAKE_AUTOMOC ON)

add_library(AppImageServicesUpdater OBJECT ${AppImageServicesUpdaterSRCS})

target_include_directories(
    AppImageServicesUpdater
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
    AppImageServicesUpdater
    PUBLIC Qt5::Core Qt5::DBus Qt5::Network libappimage CONAN_PKG::AppImageUpdaterBridge AppImageServicesCommon
)

# Install D-Bus interface descriptor
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Updater.xml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/dbus-1/interfaces
    COMPONENT AppImageServicesAPI
)
