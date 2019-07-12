#!/usr/bin/env bash

# Includes
SCRIPTS_DIR=`dirname "$0"`
source ${SCRIPTS_DIR}/debug-header.sh
source ${SCRIPTS_DIR}/settings.sh

mkdir -p "$APPLICATIONS_DIR"

APPIMAGE_SERVICES_BIN_PATH="$APPDIR/usr/bin/runtime-wrapper.sh"


cat > "$TYPE1_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH" <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=appimage-services-launcher-type-1
Comment=AppImage Type 1 Launcher
TryExec=${APPIMAGE_SERVICES_BIN_PATH}
Exec="${APPIMAGE_SERVICES_BIN_PATH}" %f 1
Icon=application-x-iso9660-appimage
Categories=Utility;
NoDisplay=true
MimeType=application/x-iso9660-appimage;
EOF

cat > "$TYPE2_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH" <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=appimage-services-launcher-type-2
Comment=AppImage Type 2 Launcher
TryExec=${APPIMAGE_SERVICES_BIN_PATH}
Exec="${APPIMAGE_SERVICES_BIN_PATH}" %f 2
Icon=application-vnd.appimage
Categories=Utility;
NoDisplay=true
MimeType=application/x-appimage;application/vnd.appimage;
EOF

chmod +x "$TYPE2_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH" "$TYPE2_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH"

echo "Updating data desktop entries database"
update-desktop-database ~/.local/share/applications/
