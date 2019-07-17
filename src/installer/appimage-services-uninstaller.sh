#!/usr/bin/env bash

# Includes
SCRIPTS_DIR=`dirname "$0"`
source ${SCRIPTS_DIR}/utils/debug-header.sh
source ${SCRIPTS_DIR}/utils/settings.sh

echo "Uninstalling AppImage Services"

# Stop systemd daemon
systemctl --user $SYSTEMD_EXTRA_ARGS stop appimage-services
systemctl --user $SYSTEMD_EXTRA_ARGS disable appimage-services

# Uninstall files
rm $SYSTEMD_USER_DIR/appimage-services.service
rm $BINDIR/appimage-services
