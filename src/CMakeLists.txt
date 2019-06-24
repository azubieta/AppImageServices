#include(export_pkg_config_path.cmake)

find_package(PkgConfig)
add_subdirectory(common)
add_subdirectory(launcher)
#add_subdirectory(updater)
add_subdirectory(inspector)
add_subdirectory(installer)

find_package(freetype REQUIRED)
find_package(Qt5 CONFIG REQUIRED Core)
add_executable(appimage-services main.cpp)
target_link_libraries(
    appimage-services
    PRIVATE freetype
    PRIVATE AppImageServicesLauncher
#    PRIVATE AppImageServicesUpdater
    PRIVATE AppImageServicesInspector
    PRIVATE Qt5::Core
)

install(
    TARGETS appimage-services
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT AppImageServices
)
install(PROGRAMS appimage-services-wrapper.sh DESTINATION ${CMAKE_INSTALL_BINDIR})
