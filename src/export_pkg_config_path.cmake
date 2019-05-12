# Workaourond cmake ignoring CMAKE_PREFIX_PATH while searching for pkg-config modules

list(JOIN CMAKE_PREFIX_PATH : PKG_CONFIG_PATH)
message("Setting PKG_CONFIG_PATH ${PKG_CONFIG_PATH}")
set(ENV{PKG_CONFIG_PATH} ${PKG_CONFIG_PATH})
