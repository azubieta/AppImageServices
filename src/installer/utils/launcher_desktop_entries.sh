#!/usr/bin/env bash

launcher_desktop_entries_create() {
  mkdir -p "$APPLICATIONS_DIR"

  echo "Installing type 1 launcher desktop entry to: $TYPE1_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH"
  cat >"$TYPE1_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH" <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=appimage-services-launcher-type-1
Comment=AppImage Type 1 Launcher
TryExec=${TYPE1_LAUNCHER_PATH}
Exec="${TYPE1_LAUNCHER_PATH}" %f 1
Icon=application-x-iso9660-appimage
Categories=Utility;
NoDisplay=true
MimeType=application/x-iso9660-appimage;
EOF

  echo "Installing type 2 launcher desktop entry to: $TYPE2_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH"
  cat >"$TYPE2_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH" <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=appimage-services-launcher-type-2
Comment=AppImage Type 2 Launcher
TryExec=${TYPE2_LAUNCHER_PATH}
Exec="${TYPE2_LAUNCHER_PATH}" %f 2
Icon=application-vnd.appimage
Categories=Utility;
NoDisplay=true
MimeType=application/x-appimage;application/vnd.appimage;
EOF

  chmod +x "$TYPE1_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH" "$TYPE2_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH"

  echo "Updating data desktop entries database"
  update-desktop-database $APPLICATIONS_DIR

  xdg-mime default org.appimage.services.launcher.type1.desktop application/x-iso9660-appimage
  xdg-mime default org.appimage.services.launcher.type2.desktop application/x-appimage application/vnd.appimage;
}

launcher_desktop_entries_remove() {
  rm -v "$TYPE1_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH" "$TYPE2_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH"

  echo "Updating data desktop entries database"
  update-desktop-database $APPLICATIONS_DIR
}
