add_executable(AppImageServicesInspectorTests AppStreamReaderTests.cpp DesktopEntryReaderTests.cpp)
target_link_libraries(AppImageServicesInspectorTests AppImageServicesInspector GTest::Main GTest::GTest)
target_add_test_data_compile_definitions(AppImageServicesInspectorTests)

target_compile_definitions(
    AppImageServicesInspectorTests
    PRIVATE "APPSTREAM_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/data/appstream.xml\""
    PRIVATE "DESKTOPENTRY_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/data/entry.desktop\""
)

gtest_discover_tests(AppImageServicesInspectorTests)
add_test(AppImageServicesInspectorTests AppImageServicesInspectorTests)