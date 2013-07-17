/*

Pencil - Traditional Animation Software
logging.cpp - Copyright (C) 2013 Manuel Odendahl <wesen@ruinwesen.com>

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include <QFile>

#include "vendor/SimpleIni/SimpleIni.h"

#include "QsLog.h"
#include "QsLogDest.h"

#include "fileutils.h"

#include "logging.h"

static QsLogging::Level logLevel(QString s) {
    if (s == "TRACE") {
        return QsLogging::TraceLevel;
    } else if (s == "DEBUG") {
        return QsLogging::DebugLevel;
    } else if (s == "INFO") {
        return QsLogging::InfoLevel;
    } else if (s == "WARN") {
        return QsLogging::WarnLevel;
    } else if (s == "ERROR") {
        return QsLogging::ErrorLevel;
    } else if (s == "FATAL") {
        return QsLogging::FatalLevel;
    } else if (s == "OFF") {
        return QsLogging::OffLevel;
    } else {
        return QsLogging::OffLevel;
    }
}

bool Logging::LoadLoggingConfig(QFile &file)
{
    static QSharedPointer<QsLogging::Destination> debugDestination(QsLogging::DestinationFactory::MakeDebugOutputDestination());

    if (file.exists() && file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();
        QString string(data);

        CSimpleIniA ini;
        ini.SetUnicode();
        std::string strData = string.toStdString();
        int rc = ini.Load(strData.c_str(), strData.size());
        if (rc >= 0) {

            CSimpleIniA::TNamesDepend sections;
            ini.GetAllSections(sections);

            QsLogging::Level defaultLevel = QsLogging::OffLevel;
            const char *value = ini.GetValue("default", "level", "OFF");
            defaultLevel = logLevel(value);
            QsLogging::Logger::setDefaultLevel(defaultLevel);

            const char *defaultConsole = ini.GetValue("default", "console", "off");
            if (QString(defaultConsole) == "on")
            {
                QsLogging::Logger::addDefaultDestination(debugDestination);
            }

            const char *defaultFile = ini.GetValue("default", "file", NULL);
            if (defaultFile)
            {
                QSharedPointer<QsLogging::Destination> fileDestination(QsLogging::DestinationFactory::MakeFileDestination(FileUtils::GetLogFilename(defaultFile)));
                QsLogging::Logger::addDefaultDestination(fileDestination);
            }

            CSimpleIniA::TNamesDepend::const_iterator i;
            for (i = sections.begin(); i != sections.end(); ++i)
            {
                const char * s = i->pItem;
                QString section(s);
                if (section == "default")
                    continue;

                CSimpleIniA::TNamesDepend keys;
                ini.GetAllKeys(s, keys);

                QsLogging::Logger& logger = QsLogging::Logger::instance(section);

                const char * value = ini.GetValue(s, "level", NULL);
                if (value) {
                    QsLogging::Level level = logLevel(value);
                    logger.setLoggingLevel(level);
                }

                value = ini.GetValue(s, "file", NULL);
                if (value) {
                    QSharedPointer<QsLogging::Destination> fileDestination(QsLogging::DestinationFactory::MakeFileDestination(FileUtils::GetLogFilename(value)));
                    logger.addDestination(fileDestination);
                }

                value = ini.GetValue(s, "console", NULL);
                if (value) {
                    if (QString(value) == "on") {
                        logger.addDestination(debugDestination);
                    } else {
                        logger.removeDestination(debugDestination);
                    }
                }
            }

            return true;
        }

        return false;
    }

    return false;

}
