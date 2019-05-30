# AppImageServices
D-Bus API over the AppImage related libraries.

This is meant to be used by file managers, software centers or other tools that aims to provide a consistent user 
experience to AppImage users. A language agnostic API (DBus) is provided to enhance compatibility with all kind of 
solutions.


The following features will be provided:
- AppImage launching
- AppImage verification
- AppImage registration (menu entries creation including icons and mime-types associations)
- AppImage un-registration
- AppImage update checking and notification
- AppImage performing the update
- AppImage metadata extraction

For more details about the API see the `README-API.md` file.


_This service must be considered an enhancement to the AppImage UX but not a mandatory requirement to use AppImages._

## Build

This project is configured to be built using [**conan**](https://conan.io) 
to install it run: 
```bash=
pip install conan
```

Add dependencies repositories:
```bash=
conan remote add appimage-conan-community https://api.bintray.com/conan/appimage-conan-community/public-conan --insert=0
conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan --insert=1
```
Then build the project using the following commands

```bash=
# on the project root dir
mkdir build && cd build
# install dependencies (may take a while if there are no 
binaries for your system)
conan install .. --build missing

# build the project
conan build ..
``` 
