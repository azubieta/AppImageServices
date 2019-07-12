#!/usr/bin/env bash

BIN_DIR=`dirname "$0"`

TARGET_APPIMAGE=$1 "$BIN_DIR/appimage-type$2-runtime" ${@:3}
