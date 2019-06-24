install(FILES appimage_logo.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps COMPONENT APPIMAGE_SERVICES)
install(FILES org.appimage.services.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications COMPONENT APPIMAGE_SERVICES)
install(FILES org.appimage.services.appdata.xml DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo COMPONENT APPIMAGE_SERVICES)
