if(ENABLE_TESTING)
    find_package(GTest REQUIRED)

    include(GoogleTest)
    include(DownloadTestsData.cmake)

    # specific module tests
    add_subdirectory(installer)
    add_subdirectory(launcher)
    add_subdirectory(inspector)

    enable_testing()
else()
    message(STATUS "Tests are disabled use '-DENABLE_TESTING:Bool=True' to enable them")
endif()
