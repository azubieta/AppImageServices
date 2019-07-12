#!/usr/bin/env bash

set -ex

conan remote add appimage-conan-community https://api.bintray.com/conan/appimage-conan-community/public-conan --insert=0
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan --insert=1
# Original cmake binaries are faulty forcing re-build
conan install cmake_installer/3.14.3@conan/stable --build cmake_installer

mkdir -p /tmp/AppImageServices-build
pushd /tmp/AppImageServices-build

conan install $SRCS_DIR --build missing
conan build $SRCS_DIR

mkdir -p $SRCS_DIR/dist

mv *.AppImage* $SRCS_DIR/dist
popd
