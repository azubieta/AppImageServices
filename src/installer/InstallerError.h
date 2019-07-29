#pragma once

#include <QString>

class InstallerError : public std::runtime_error {
public:
    explicit InstallerError(const QString &arg);
};