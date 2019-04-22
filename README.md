# AppImageService
Language agnostic implementation of the AppImage user experience (not mandatory to use AppImages)

This is meant to be used by file managers, software centers or other tools that aims to provide a consistent user 
experience to AppImage users. A language agnostic API (DBus) is provided to enhance compatibility with all kind of 
solutions.


The following features will be provided:
- launching
- verification
- registration (menu entries creation including icons and mime-types associations)
- un-registration
- update checking and notification
- performing the update
- metadata extraction

_This service must be considered an enhancement to the AppImage UX but not a mandatory requirement to use AppImages._
