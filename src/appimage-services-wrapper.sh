#!/bin/bash

display_usage() {
  echo
  echo "Usage: $0"
  echo
  echo " -h, --help   Display usage instructions"
  echo " launch <target appimage> <target appimage type>, Start an AppImage using the embedded runtime"
  echo " install <AppImage path>, Install the AppImage in the system"
  echo " uninstall <AppImage installed path>, Uninstall the AppImage from the system"
  echo
  echo " self-install, Install AppImageServices on the system"
  echo " self-uninstall, Uninstall AppImageServices from the system"
  echo
}

raise_error() {
  local error_message="$@"
  echo "${error_message}" 1>&2;
}


# environment setup
export PATH=$PATH:$APPDIR/usr/bin

# arguments parsing
command="$1"
if [[ -z $command ]] ; then
  echo "Updating launchers desktop entries"
  "$APPDIR/usr/bin/utils/update_launchers_desktop_entries.sh"

  echo "Launching daemon"
  $APPDIR/usr/bin/appimage-services
else
  case $command in
    -h|--help)
      display_usage
      ;;
        install)
      "$APPDIR/usr/bin/install-helper" "${@:2}"
      ;;
        uninstall)
      "$APPDIR/usr/bin/uninstall-helper" "${@:2}"
      ;;
    self-install)
      "$APPDIR/usr/bin/appimage-services-installer.sh"
      ;;
    self-uninstall)
      "$APPDIR/usr/bin/appimage-services-uninstaller.sh"
      ;;
    *)
      raise_error "Unknown command: ${command}"
      display_usage
      ;;
  esac
fi
