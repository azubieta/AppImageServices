find_package(Qt5 CONFIG REQUIRED Core DBus)

# Generate the xml interface file from DBusInterface.h
qt5_generate_dbus_interface(
    # header
    DBusInterface.h
    # interfacename
    org.appimage.Services.Launcher.xml
    # aditional qdbuscpp2xml options
    OPTIONS -A
)

# Create a dbus adaptor (header and implementation file) from the xml file
qt5_add_dbus_adaptor(
    AppImageServiceSRCS
    # xmlfile
    ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Launcher.xml
    # parentheader
    DBusInterface.h
    # parentclassname
    DBusInterface
)
set(CMAKE_AUTOMOC ON)

add_executable(AppImageService ${AppImageServiceSRCS} main.cpp DBusInterface.cpp)

target_include_directories(
    AppImageService
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(AppImageService PRIVATE Qt5::Core Qt5::DBus)

install(
    TARGETS AppImageService
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT AppImageService
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Launcher.xml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/dbus-1/interfaces
    COMPONENT AppImageServiceAPI
)
