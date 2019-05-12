#!/usr/bin/env python
# -*- coding: utf-8 -*-

from conans import ConanFile, AutoToolsBuildEnvironment, tools
import os


class AppImageServices(ConanFile):
    name = "AppImageServices"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"

    requires = (("libpng/1.6.36@bincrafters/stable"),
                ("lzma/5.2.4@bincrafters/stable"),
                ("glib/2.40.0@appimage-conan-community/stable"),
                ("qt/5.12.3@appimage-conan-community/stable"),
                ("xdg-utils-cxx/0.1.1@appimage-conan-community/stable"),
                ("libappimage/1.0.0@appimage-conan-community/stable"),
                ("squashfuse/0.1.103@appimage-conan-community/stable"))

    default_options = {"glib:shared": True, "lzma:shared": True}


    build_requires = ("patchelf_installer/0.9@appimage-conan-community/stable",
                      "gtest/1.8.1@bincrafters/stable",
                      "glib/2.40.0@appimage-conan-community/stable",
                      "cmake_installer/3.14.3@conan/stable",
                      "linuxdeploy/continuous@appimage-conan-community/stable",
                      "linuxdeploy-plugin-qt/continuous@appimage-conan-community/stable",
                      "linuxdeploy-plugin-appimage/continuous@appimage-conan-community/stable")

    generators = "cmake_paths", "qt", "virtualrunenv"

    def imports(self):
        # Patch squashfuse prefix. Wonder when cmake will support 'pkg-config --define-prefix'
        squashfuse_path = self.deps_cpp_info["squashfuse"].rootpath
        tools.replace_prefix_in_pc_file(os.path.join(squashfuse_path, "lib", "pkgconfig", "squashfuse.pc"),
                                        squashfuse_path)
