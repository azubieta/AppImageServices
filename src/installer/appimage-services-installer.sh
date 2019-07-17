#!/usr/bin/env bash

# Includes
SCRIPTS_DIR=`dirname "$0"`
source ${SCRIPTS_DIR}/utils/debug-header.sh
source ${SCRIPTS_DIR}/utils/settings.sh

echo "Intalling binary to $BINDIR/appimage-services"
mkdir -p $BINDIR
cp --force $APPIMAGE $BINDIR/appimage-services

echo "Installing systemd service to $SYSTEMD_USER_DIR/appimage-services.service"
mkdir -p $SYSTEMD_USER_DIR
cat > "$SYSTEMD_USER_DIR/appimage-services.service" <<EOF
[Unit]
Description=AppImage Services

[Service]
ExecStart=$BINDIR/appimage-services
Restart=on-failure
RestartSec=10

[Install]
WantedBy=default.target
EOF

systemctl --user $SYSTEMD_EXTRA_ARGS daemon-reload
systemctl --user $SYSTEMD_EXTRA_ARGS enable appimage-services
systemctl --user $SYSTEMD_EXTRA_ARGS restart appimage-services

if [ ! $KEEP_INSTALLER ]; then
    rm --verbose $APPIMAGE
fi
