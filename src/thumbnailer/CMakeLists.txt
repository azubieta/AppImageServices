find_package(libappimage REQUIRED)
find_package(Qt5 CONFIG REQUIRED Core DBus)

set(
    AppImageServicesThumbnailerSRCS
    ThumbnailerDBusInterface.cpp
)

# Generate the xml interface file from LauncherDBusInterface.h
qt5_generate_dbus_interface(
    # header
    ThumbnailerDBusInterface.h
    # interfacename
    org.appimage.Services.Thumbnailer.xml
    # aditional qdbuscpp2xml options
    OPTIONS -A
)

# Create a launcher adaptor (header and implementation file) from the xml file
qt5_add_dbus_adaptor(
    AppImageServicesThumbnailerSRCS
    # xmlfile
    ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Thumbnailer.xml
    # parentheader
    ThumbnailerDBusInterface.h
    # parentclassname
    ThumbnailerDBusInterface
)
set(CMAKE_AUTOMOC ON)


add_library(AppImageServicesThumbnailer OBJECT ${AppImageServicesThumbnailerSRCS})

target_include_directories(
    AppImageServicesThumbnailer
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
    AppImageServicesThumbnailer
    PUBLIC Qt5::DBus Qt5::Core libappimage AppImageServicesCommon
)

# Install D-Bus interface descriptor
install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/org.appimage.Services.Thumbnailer.xml
    DESTINATION ${CMAKE_INSTALL_DATADIR}/dbus-1/interfaces
    COMPONENT AppImageServicesAPI
)
