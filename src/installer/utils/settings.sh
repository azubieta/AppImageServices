#!/usr/bin/env bash

if [ "$EUID" -ne 0 ]; then
  # Non root installation
  BINDIR=$HOME/.local/bin
  CONFIG_DIR=$HOME/.config
  DATA_DIR=$HOME/.local/share
  AUTOSTART_DIR=$CONFIG_DIR/autostart
else
  # Root installation
  BINDIR=/usr/local/bin
  CONFIG_DIR=/etc
  DATA_DIR=/usr/share
  AUTOSTART_DIR=$CONFIG_DIR/xdg/autostart
fi

APPLICATIONS_DIR=$DATA_DIR/applications
SYSTEMD_USER_DIR=$CONFIG_DIR/systemd/user
DBUS_SYSTEM_SERVICES_DIR=$DATA_DIR/dbus-1/services

TYPE1_LAUNCHER_PATH="$BINDIR/appimage-services-launcher-type-1"
TYPE2_LAUNCHER_PATH="$BINDIR/appimage-services-launcher-type-2"

TYPE1_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH="${APPLICATIONS_DIR}/org.appimage.services.launcher.type1.desktop"
TYPE2_LAUNCHER_DESKTOP_ENTRY_TARGET_PATH="${APPLICATIONS_DIR}/org.appimage.services.launcher.type2.desktop"

# Configuration independent paths
BINFMT_DIR=/usr/lib/binfmt.d/

KEEP_INSTALLER=true
