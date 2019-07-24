#!/usr/bin/env bash
set -e
# Builds the AppImageServices AppImage inside a docker container. It expects to be executed from the project root dir.
# Results will be placed at: docker_build/

# Upgrade Conan version
sudo pip install --upgrade conan
# Automatic detection of your arch, compiler, etc.
conan user

# Add conan repositories
conan remote add appimage-conan-community https://api.bintray.com/conan/appimage-conan-community/public-conan --insert=0
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan --insert=1

# Original cmake binaries are faulty forcing re-build
conan install cmake_installer/3.14.3@conan/stable --build cmake_installer

mkdir -p docker_build
cd docker_build

conan install .. --build missing -s compiler.libcxx=libstdc++11 -s compiler.cppstd=11
conan build ..
