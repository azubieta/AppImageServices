#!/usr/bin/env bash

if [ "$EUID" -ne 0 ]; then
    # Non root installation
    BINDIR=$HOME/.local/bin
    AUTOSTART_DIR=$HOME/.config/autostart
else
    # Root installation
    BINDIR=/usr/local/bin
    AUTOSTART_DIR=/etc/xdg/autostart
fi

KEEP_INSTALLER=true
