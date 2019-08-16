#!/usr/bin/env bash

BIN_DIR=`dirname "$0"`

"$BIN_DIR/appimage-type$2-runtime" $1 ${@:3}
