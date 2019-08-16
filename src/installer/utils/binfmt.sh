#!/usr/bin/env bash

binfmt_register() {
  # binfmt is only available when installed as root
  if [ "$EUID" -eq 0 ]; then
    echo "Install binfmt entries"
    if [ $(command -v update-binfmts) -eq 0 ]; then
      update-binfmts --package appimage --install appimage-type1 $BINDIR/appimage-services-launcher-type-1 --magic 'AI\x01' --offset 8
      update-binfmts --package appimage --install appimage-type2 $BINDIR/appimage-services-launcher-type-2 --magic 'AI\x02' --offset 8
    else
      echo ":appimage-type1:M:8:AI\x01::@CMAKE_INSTALL_PREFIX@/bin/AppImageLauncher:" >>$BINFMT_DIR/org.appimage.conf
      echo ":appimage-type2:M:8:AI\x02::@CMAKE_INSTALL_PREFIX@/bin/AppImageLauncher:" >>$BINFMT_DIR/org.appimage.conf
    fi

    systemctl restart binfmt-support.service
  fi
}

binfmt_remove() {
  # binfmt is only available when installed as root
  if [ "$EUID" -eq 0 ]; then
    echo "Remove binfmt entries"
    if [ $(command -v update-binfmts) -eq 0 ]; then
      update-binfmts --package appimage --remove appimage-type1 $BINDIR/appimage-services-launcher-type-1
      update-binfmts --package appimage --remove appimage-type2 $BINDIR/appimage-services-launcher-type-2
    else
      # update-binfmts is not available on all systems therefore systemd-binfmt config files are used
      rm -v $BINFMT_DIR/org.appimage.conf
    fi

    systemctl restart binfmt-support.service
  fi
}
