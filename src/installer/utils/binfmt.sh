#!/usr/bin/env bash

binfmt_register() {
  # binfmt is only available when installed as root
  if [ "$EUID" -eq 0 ]; then
    echo "Install binfmt entries"
    if [ $(command -v update-binfmts) -eq 0 ]; then
      update-binfmts --package appimage --install appimage-type1 $TYPE1_LAUNCHER_PATH --magic 'AI\x01' --offset 8
      update-binfmts --package appimage --install appimage-type2 $TYPE2_LAUNCHER_PATH --magic 'AI\x02' --offset 8
    else
      echo ":appimage-type1:M:8:AI\x01::$TYPE1_LAUNCHER_PATH:" >> $BINFMT_DIR/org.appimage.conf
      echo ":appimage-type2:M:8:AI\x02::$TYPE2_LAUNCHER_PATH:" >> $BINFMT_DIR/org.appimage.conf
    fi

    systemctl restart systemd-binfmt.service
  fi
}

binfmt_remove() {
  # binfmt is only available when installed as root
  if [ "$EUID" -eq 0 ]; then
    echo "Remove binfmt entries"
    if [ $(command -v update-binfmts) -eq 0 ]; then
      update-binfmts --package appimage --remove appimage-type1 $TYPE1_LAUNCHER_PATH
      update-binfmts --package appimage --remove appimage-type2 $TYPE2_LAUNCHER_PATH
    else
      # update-binfmts is not available on all systems therefore systemd-binfmt config files are used
      rm -v $BINFMT_DIR/org.appimage.conf
    fi

    systemctl restart systemd-binfmt.service
  fi
}
