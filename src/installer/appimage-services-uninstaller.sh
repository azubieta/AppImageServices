#!/usr/bin/env bash

# Includes
SCRIPTS_DIR=`dirname "$0"`
source ${SCRIPTS_DIR}/utils/debug-header.sh
source ${SCRIPTS_DIR}/utils/settings.sh

echo "Uninstalling $BINDIR/appimage-services.AppImage"

# Uninstall files
rm $AUTOSTART_DIR/org.appimage.services.desktop
rm $BINDIR/appimage-services.AppImage

rm "$TYPE1_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH"
rm "$TYPE2_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH"
