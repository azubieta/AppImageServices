#!/usr/bin/env bash

set -xe

build_scripts_dir=`dirname $0`
source_dir=`dirname $build_scripts_dir`

echo $source_dir
conan install $source_dir --build missing

. activate_run.sh

cmake $source_dir -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PROJECT_AppImageServices_INCLUDE=$PWD/conan_paths.cmake
make install -j`nproc` DESTDIR=AppDir
linuxdeploy --appdir=AppDir --plugin qt --output appimage --desktop-file=AppDir/usr/share/applications/org.appimage.services.desktop

. deactivate_run.sh
