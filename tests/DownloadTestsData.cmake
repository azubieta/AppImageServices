set(TESTS_DATA_DIR "${CMAKE_CURRENT_BINARY_DIR}/data")

if(NOT EXISTS ${TESTS_DATA_DIR})
    file(MAKE_DIRECTORY ${TESTS_DATA_DIR})
endif()

set(TESTS_DATA_TYPE2_APPIMAGE "${TESTS_DATA_DIR}/type2_x86_64.AppImage")
if(NOT EXISTS ${TESTS_DATA_TYPE2_APPIMAGE})
    file(
        DOWNLOAD https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
        ${TESTS_DATA_TYPE2_APPIMAGE} SHOW_PROGRESS
    )
endif()


function(target_add_test_data_compile_definitions target)
    target_compile_definitions(${target}
        PRIVATE "TESTS_DATA_DIR=\"${TESTS_DATA_DIR}\""
        PRIVATE "TESTS_DATA_TYPE2_APPIMAGE=\"${TESTS_DATA_TYPE2_APPIMAGE}\"")
endfunction()
