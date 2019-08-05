if(ENABLE_TESTING)
    find_package(GTest REQUIRED)

    include(GoogleTest)
    include(DownloadTestsData.cmake)

    # specific module tests
    add_subdirectory(installer)

    enable_testing()
else()
    message(STATUS "Tests are disabled use '-DENABLE_TESTING:Bool=True' to enable them")
endif()
