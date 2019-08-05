#include "InstallerError.h"

InstallerError::InstallerError(const QString &arg) : runtime_error(arg.toStdString()) {}
