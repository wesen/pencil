/*

Pencil - Traditional Animation Software
fileutils.cpp - Copyright (C) 2013 Manuel Odendahl <wesen@ruinwesen.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include <QTextStream>
#include <QApplication>
#include <QDesktopServices>
#include <QMutex>

#include "fileutils.h"

//==============================================================================
// Delete a directory along with all of its contents.
//
// Adapted from: http://john.nachtimwald.com/2010/06/08/qt-remove-directory-and-its-contents/
//==============================================================================
bool FileUtils::RemoveDir(const QString &dirName, bool keepRoot)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists())
    {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir())
            {
                result = RemoveDir(info.absoluteFilePath());
            }
            else
            {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result)
            {
                return result;
            }
        }

        if (!keepRoot)
        {
            result = dir.rmdir(dirName);
        }
    }

    return result;
}

bool FileUtils::EmptyDir(const QString &dirName)
{
    return RemoveDir(dirName, true);
}

void FileUtils::EnsureDirectoryExists(const QString &dirName)
{
    QDir dir(dirName);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            throw new DirectoryNotFoundException("Could not create " + dir.absolutePath());
        }
    }
}

QString FileUtils::FileContents(const QString &fileName)
{
    QString fileContent = "";
    QFile textFile(fileName);

    if (textFile.open(QIODevice::ReadOnly))
    {
        QTextStream textStream(&textFile);
        QString line;
        while (!(line = textStream.readLine()).isNull())
        {
            fileContent += line + "\n";
        }

        textFile.close();
    }

    return fileContent;
}

QDir FileUtils::GetApplicationDirectory() {
    return QDir(QApplication::applicationDirPath());
}

QDir FileUtils::GetApplicationDirectory(QString name) {
    QDir dir = GetApplicationDirectory();
    QDir dir2 = QDir(dir.absoluteFilePath(name));
    if (!dir2.exists()) {
        if (!dir2.mkpath(".")) {
            throw new DirectoryNotFoundException("Could not create " + dir2.absolutePath());
        }
    }
    return dir2.absolutePath();
}

QDir FileUtils::GetSettingsDirectory() {
#ifdef Q_OS_MAC
    return GetResourceDirectory("Settings");
#else
    return GetApplicationDirectory();
#endif
}

QString FileUtils::GetSettingsFilename(QString name) {
    QDir dir = GetSettingsDirectory();
    return "crypt:" + dir.absoluteFilePath(name);
}

QDir FileUtils::GetLogDirectory()
{
    return GetDataDirectory("Logs");
}

QString FileUtils::GetLogFilename(QString name)
{
    QDir dir = GetLogDirectory();
    QString res = dir.absoluteFilePath(name);
    return res;
}

QDir FileUtils::GetUserSettingsDirectory()
{
    return GetDataDirectory("Settings");
}

QString FileUtils::GetUserSettingsFilename(QString name)
{
    QDir dir = GetUserSettingsDirectory();
    QString res = dir.absoluteFilePath(name);
    return res;
}

QDir FileUtils::GetDataDirectory() {
    return QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation));
}

QDir FileUtils::GetDataDirectory(QString name) {
    QDir dir = GetDataDirectory();
    QDir dir2 = QDir(dir.absoluteFilePath(name));
    if (!dir2.exists()) {
        if (!dir2.mkpath(".")) {
            throw new DirectoryNotFoundException("Could not create " + dir2.absolutePath());
        }
    }
    return dir2.absolutePath();
}

QString FileUtils::GetDataFilename(QString name) {
    QDir dir = GetDataDirectory();
    return dir.absoluteFilePath(name);
}

QString FileUtils::GetDataFilename(QString name, QString subdir) {
    QDir dir = GetDataDirectory(subdir);
    return dir.absoluteFilePath(name);
}

QDir FileUtils::GetResourceDirectory() {
#ifdef Q_OS_MAC
    return GetApplicationDirectory("../Resources/");
#else
    return GetApplicationDirectory("Resources");
#endif
}

QDir FileUtils::GetResourceDirectory(QString name) {
    QDir dir = GetResourceDirectory();
    QDir dir2 = QDir(dir.absoluteFilePath(name));
    if (!dir2.exists()) {
        if (!dir2.mkpath(".")) {
            throw new DirectoryNotFoundException("Could not create " + dir2.absolutePath());
        }
    }
    return dir2.absolutePath();
}

QString FileUtils::GetResourceFilename(QString name) {
    QDir dir = GetResourceDirectory();
    return dir.absoluteFilePath(name);
}

QString FileUtils::GetResourceFilename(QString name, QString subdir) {
    QDir dir = GetResourceDirectory(subdir);
    return dir.absoluteFilePath(name);
}

// All temp files for preform will be under pencil2d
QDir FileUtils::GetTempDirectory() {
    QDir temp(QDesktopServices::storageLocation(QDesktopServices::TempLocation));
    QDir temp2 = QDir(temp.absoluteFilePath("pencil2d"));

    if (!temp2.exists()) {
        if (!temp2.mkpath(".")) {
            throw new DirectoryNotFoundException("Could not create " + temp2.absolutePath());
        }
    }

    return temp2.absolutePath();
}

QDir FileUtils::GetTempDirectory(QString name) {
    QDir dir = GetTempDirectory();
    QDir dir2 = QDir(dir.absoluteFilePath(name));
    if (!dir2.exists()) {
        if (!dir2.mkpath(".")) {
            throw new DirectoryNotFoundException("Could not create " + dir2.absolutePath());
        }
    }
    return dir2.absolutePath();
}

QString FileUtils::GetTempFilename(QString name) {
    return GetTempFilename(".", name);
}

QString FileUtils::GetTempFilename(QString subdir, QString name) {
    QDir dir = GetTempDirectory(subdir);
    if (dir.exists()) {
        return dir.absoluteFilePath(name);
    } else {
        throw new DirectoryNotFoundException("Could not create " + dir.absolutePath());
    }
}

QString FileUtils::GetTempFilename()
{
    static QMutex tempFileAcquireMutex;

    // For safe parallel access
    tempFileAcquireMutex.lock();

    QDir temp = GetTempDirectory();
    QString name = "temp";
    while (temp.exists(name))
    {
        name += QString::number(rand() % 10);
    }

    // Create the file
    QFile f(temp.absoluteFilePath(name));
    f.open(QIODevice::ReadOnly);
    f.close();

    tempFileAcquireMutex.unlock();

    return temp.absoluteFilePath(name);
}
