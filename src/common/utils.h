#pragma once

#include <QString>

/**
 * @brief Remove protocol section from local paths URI
 *
 * Examples:
 * - `file:///path/to/file` -> `/path/to/file`
 * - `/path/to/file` -> `/path/to/file`
 *
 * @param uri
 * @return path without the protocol header ('file://')
 */
QString removeUriProtocolFromPath(const QString& uri);

/**
 * Resolves the application settings file path from the XDG_CONFIG_DIRS
 * @return settings File Path
 */
QString settingsFilePath();