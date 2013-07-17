/*

Pencil - Traditional Animation Software
fileutils.h - Copyright (C) 2013 Manuel Odendahl <wesen@ruinwesen.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <stdexcept>

#include <QString>
#include <QDir>

class FileNotFoundException: public std::runtime_error {
public:
    FileNotFoundException(QString str) : std::runtime_error(str.toStdString()) {
    }
};

class DirectoryNotFoundException: public std::runtime_error {
public:
    DirectoryNotFoundException(QString str) : std::runtime_error(str.toStdString()) {
    }
};

class FileUtils
{
public:
    static bool RemoveDir(const QString &dirName, bool keepRoot = false);
    static bool EmptyDir(const QString &dirName);
    static QString FileContents(const QString &fileName);
    static void EnsureDirectoryExists(const QString &dirName);

    static QDir GetApplicationDirectory();
    static QDir GetApplicationDirectory(QString name);

    // fixed settings within the program
    static QDir GetSettingsDirectory();
    static QString GetSettingsFilename(QString name);

    // user modifiable settings (besides QSettings)
    static QDir GetUserSettingsDirectory();
    static QString GetUserSettingsFilename(QString name);

    // log directory (in Application Support / Application Data)
    static QDir GetLogDirectory();
    static QString GetLogFilename(QString name);

    // user modifiable and storable data
    static QDir GetDataDirectory();
    static QDir GetDataDirectory(QString name);
    static QString GetDataFilename(QString name);
    static QString GetDataFilename(QString subdir, QString name);

    // static data released with the program
    static QDir GetResourceDirectory();
    static QDir GetResourceDirectory(QString name);
    static QString GetResourceFilename(QString name);
    static QString GetResourceFilename(QString subdir, QString name);

    // temporary directory (in users Application Data)
    static QDir GetTempDirectory();
    static QDir GetTempDirectory(QString name);
    static QString GetTempFilename(QString subdir, QString name);
    static QString GetTempFilename(QString name);
    static QString GetTempFilename();

private:
    FileUtils() {
    }
};

#endif // FILEUTILS_H
