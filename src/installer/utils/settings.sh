#!/usr/bin/env bash

if [ "$EUID" -ne 0 ]; then
    # Non root installation
    BINDIR=$HOME/.local/bin
    AUTOSTART_DIR=$HOME/.config/autostart
    APPLICATIONS_DIR=$HOME/.local/share/applications/
else
    # Root installation
    BINDIR=/usr/local/bin
    AUTOSTART_DIR=/etc/xdg/autostart
    APPLICATIONS_DIR=/usr/share/applications/
fi

TYPE1_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH="${APPLICATIONS_DIR}/org.appimage.services.launcher.type1.desktop"
TYPE2_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH="${APPLICATIONS_DIR}/org.appimage.services.launcher.type2.desktop"

KEEP_INSTALLER=true
