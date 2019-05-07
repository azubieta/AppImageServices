add_subdirectory(launcher)

find_package(Qt5 CONFIG REQUIRED Core)
add_executable(appimage-services main.cpp)
target_link_libraries(appimage-services PRIVATE AppImageServicesLauncher Qt5::Core)

install(
    TARGETS appimage-services
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT AppImageServices
)
