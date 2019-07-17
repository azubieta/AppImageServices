#!/usr/bin/env bash

if [ "$EUID" -ne 0 ]; then
    # Non root installation
    BINDIR=$HOME/.local/bin
    APPLICATIONS_DIR=$HOME/.local/share/applications
    CONFIG_DIR=$HOME/.config
    AUTOSTART_DIR=$CONFIG_DIR/autostart
    DATA_DIR=$HOME/.local/share
    SYSTEMD_USER_DIR=$CONFIG_DIR/systemd/user
    DBUS_SYSTEM_SERVICES_DIR=$DATA_DIR/dbus-1/services
else
    # Root installation
    BINDIR=/usr/local/bin
    APPLICATIONS_DIR=/usr/share/applications
    CONFIG_DIR=/etc
    AUTOSTART_DIR=$CONFIG_DIR/xdg/autostart
    DATA_DIR=/usr/share
    SYSTEMD_USER_DIR=$CONFIG_DIR/systemd/user
    DBUS_SYSTEM_SERVICES_DIR=$DATA_DIR/dbus-1/services
fi

TYPE1_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH="${APPLICATIONS_DIR}/org.appimage.services.launcher.type1.desktop"
TYPE2_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH="${APPLICATIONS_DIR}/org.appimage.services.launcher.type2.desktop"

KEEP_INSTALLER=true
