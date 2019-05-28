#!/usr/bin/env python
# -*- coding: utf-8 -*-

from conans import ConanFile, CMake, tools


class AppImageServices(ConanFile):
    name = "AppImageServices"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"

    requires = (("OpenSSL/1.1.1b@conan/stable"),
                ("libpng/1.6.36@bincrafters/stable"),
                ("cairo/1.17.2@bincrafters/stable"),
                ("glib/2.40.0@appimage-conan-community/stable"),
                ("qt/5.12.3@appimage-conan-community/stable"),
                ("xdg-utils-cxx/0.1.1@appimage-conan-community/stable"),
                ("libappimage/1.0.1@appimage-conan-community/stable"),
                ("appimageupdate/continuous@appimage-conan-community/stable"),
                ("squashfuse/0.1.103@appimage-conan-community/stable"))

    default_options = {"glib:shared": True, "lzma:shared": True}

    build_requires = ("patchelf_installer/0.9@appimage-conan-community/stable",
                      "gtest/1.8.1@bincrafters/stable",
                      "glib/2.40.0@appimage-conan-community/stable",
                      "cmake_installer/3.14.3@conan/stable",
                      "linuxdeploy/continuous@appimage-conan-community/stable",
                      "linuxdeploy-plugin-qt/continuous@appimage-conan-community/stable",
                      "linuxdeploy-plugin-appimage/continuous@appimage-conan-community/stable")

    generators = "cmake_paths", "pkg_config"

    def build(self):
        appDirPath = self.build_folder + "/AppDir"

        cmake = CMake(self)
        cmake.definitions["CMAKE_PROJECT_AppImageServices_INCLUDE"] = self.build_folder + "/conan_paths.cmake"
        # Correct conan default installation paths
        cmake.definitions["CMAKE_INSTALL_PREFIX"] = appDirPath
        cmake.definitions["CMAKE_INSTALL_BINDIR"] = "usr/bin"
        cmake.definitions["CMAKE_INSTALL_LIBDIR"] = "usr/lib"
        cmake.definitions["CMAKE_INSTALL_INCLUDEDIR"] = "usr/include"
        cmake.definitions["CMAKE_INSTALL_DATAROOTDIR"] = "usr/share"
        cmake.definitions["CMAKE_INSTALL_SYSCONFDIR"] = "etc"

        cmake.configure()
        cmake.build()
        cmake.install()

        self.run("linuxdeploy --appdir=%s --plugin qt --output appimage" % appDirPath, run_environment=True)
