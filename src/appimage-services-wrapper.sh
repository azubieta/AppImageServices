#!/bin/bash

echo "Starting sctipt $0"

display_usage() {
  echo
  echo "Usage: $0"
  echo
  echo " -h, --help   Display usage instructions"
  echo " launch <target appimage> <target appimage type>, Start an AppImage using the embedded runtime"
  echo " self-install, Install the AppImage in the system"
  echo " self-uninstall, Install the AppImage in the system"
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
  appimage-services
else
  case $command in
    -h|--help)
      display_usage
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
