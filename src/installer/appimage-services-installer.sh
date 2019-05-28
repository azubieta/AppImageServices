#!/usr/bin/env bash

# Includes
SCRIPTS_DIR=`dirname "$0"`
source ${SCRIPTS_DIR}/utils/debug-header.sh
source ${SCRIPTS_DIR}/utils/settings.sh

echo "Installing $APPIMAGE"

# Create installation directories
mkdir -p $BINDIR
mkdir -p $SYSTEMD_USER_DIR

# Install files
cp --verbose --force $APPIMAGE $BINDIR/appimage-services.AppImage

cp --verbose --force \
    $APPDIR/usr/share/applications/org.appimage.services.desktop \
    $AUTOSTART_DIR/org.appimage.services.desktop

sed -i "s|\Exec=.*|Exec=$BINDIR/appimage-services.AppImage|" $AUTOSTART_DIR/org.appimage.services.desktop
sed -i "s|\TryExec=.*|TryExec=$BINDIR/appimage-services.AppImage|" $AUTOSTART_DIR/org.appimage.services.desktop

echo ""
echo "To start the service reboot your system or run:"
echo ""
echo "$BINDIR/appimage-services.AppImage &"

if [ ! $KEEP_INSTALLER ]; then
    rm --verbose $APPIMAGE
fi
