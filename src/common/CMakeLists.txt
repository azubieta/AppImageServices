find_package(Qt5 CONFIG REQUIRED Core)

add_library(AppImageServicesCommon STATIC utils.cpp)
target_link_libraries(AppImageServicesCommon PUBLIC Qt5::Core)
target_include_directories(AppImageServicesCommon PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
