#!/usr/bin/env python
# -*- coding: utf-8 -*-

from conans import ConanFile, CMake, tools
from shutil import copyfile
import os


class AppImageServices(ConanFile):
    name = "AppImageServices"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"

    requires = (("OpenSSL/1.1.1b@conan/stable"),
                ("harfbuzz/2.4.0@appimage-conan-community/stable"),
                ("glib/2.40.0@appimage-conan-community/stable"),
                ("libpng/1.6.36@bincrafters/stable"),
                ("cairo/1.17.2@appimage-conan-community/stable"),
                ("qt/5.12.3@appimage-conan-community/stable"),
                ("xdg-utils-cxx/0.1.1@appimage-conan-community/stable"),
                ("freetype/2.9.1@appimage-conan-community/stable"),
                ("libappimage/1.0.2@local/testing"),
                ("AppImageUpdaterBridge/1.0.4@appimage-conan-community/stable"),
                ("squashfuse/0.1.103@appimage-conan-community/stable"))

    default_options = {
        "squashfuse:shared": False,
        "libarchive:shared": False,
        "xdg-utils-cxx:shared": False,
        "xdg-utils-cxx:fPIC": True,
        "cairo:shared": True,
        "pango:shared": True,
        "librsvg:shared": True,
        "freetype:shared": True,
        "glib:shared": True,
        "lzma:shared": True,
        "zlib:shared": True,
        "qt:shared": True,
        "AppImageUpdaterBridge:shared": True,
        "freetype:shared": True}

    build_requires = ("patchelf_installer/0.9@appimage-conan-community/stable",
                      "gtest/1.8.1@bincrafters/stable",
                      "glib/2.40.0@appimage-conan-community/stable",
                      "cmake_installer/3.14.3@conan/stable",
                      "linuxdeploy/continuous@appimage-conan-community/stable",
                      "linuxdeploy-plugin-qt/continuous@appimage-conan-community/stable",
                      "linuxdeploy-plugin-appimage/continuous@appimage-conan-community/stable")

    generators = "cmake", "cmake_paths", "pkg_config"

    def import_pkg_config_files(self, pkg, pkgconfig_path):
        for root, dirs, files in os.walk(self.deps_cpp_info[pkg].rootpath):
            for file in files:
                if file.endswith("pc"):
                    source_path = os.path.join(root, file)
                    target_path = os.path.join(pkgconfig_path, file)
                    print("Importing pkg_config file: %s" % target_path)
                    copyfile(source_path, target_path)
                    tools.replace_prefix_in_pc_file(target_path, self.deps_cpp_info[pkg].rootpath)

    def build(self):
        appDirPath = self.build_folder + "/AppDir"

        for lib in self.deps_cpp_info.deps:
            self.import_pkg_config_files(lib, self.build_folder)

        with tools.environment_append({'PKG_CONFIG_PATH': self.build_folder}):
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
