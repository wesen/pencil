// Copyright (c) 2013, Razvan Petru
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef QSLOG_H
#define QSLOG_H

#include "QsLogLevel.h"
#include <QDebug>
#include <QString>
#include <QMap>
#include <QSharedPointer>

#define QS_LOG_LINE_NUMBERS

namespace QsLogging
{
class Destination;
class LoggerImpl; // d pointer

class Logger
{
public:
    static Logger& instance()
    {
        return instance("main");
    }

    static void addDefaultDestination(QSharedPointer<Destination> destination);
    static QVector<QSharedPointer<Destination> > getDefaultDestinations();
    static void setDefaultLevel(Level level);

    static Logger& instance(const QString &name);

    //! Adds a log message destination. Don't add null destinations.
    void addDestination(QSharedPointer<Destination> destination);
    //! Removes a log message destination.
    void removeDestination(QSharedPointer<Destination> destination);
    //! Logging at a level < 'newLevel' will be ignored
    void setLoggingLevel(Level newLevel);
    //! The default level is INFO
    Level loggingLevel() const;

    //! The helper forwards the streaming to QDebug and builds the final
    //! log message.
    class Helper
    {
    public:
        explicit Helper(const QString &_name, Level logLevel) :
            level(logLevel),
            name(_name),
            qtDebug(&buffer) {}
        ~Helper();
        QDebug& stream(){ return qtDebug; }

    private:
        void writeToLog();

        Level level;
        QString name;
        QString buffer;
        QDebug qtDebug;
    };

private:
    Logger();
    Logger(const Logger&);
    Logger& operator=(const Logger&);
    ~Logger();

    void write(const QString& message, Level level);

    LoggerImpl* d;
};

} // end namespace

//! Logging macros: define QS_LOG_LINE_NUMBERS to get the file and line number
//! in the log output.
#ifndef QS_LOG_LINE_NUMBERS
#ifdef NDEBUG
#define QLOG_TRACE(n)    {}
#define QLOG_DEBUG(n)    {}
#else
#define QLOG_TRACE(n) \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::TraceLevel ){} \
    else QsLogging::Logger::Helper((n), QsLogging::TraceLevel).stream()
#define QLOG_DEBUG(n) \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::DebugLevel ){} \
    else QsLogging::Logger::Helper((n), QsLogging::DebugLevel).stream()
#endif
#define QLOG_INFO(n)  \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::InfoLevel ){} \
    else QsLogging::Logger::Helper((n), QsLogging::InfoLevel).stream()
#define QLOG_WARN(n)  \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::WarnLevel ){} \
    else QsLogging::Logger::Helper((n), QsLogging::WarnLevel).stream()
#define QLOG_ERROR(n) \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::ErrorLevel ){} \
    else QsLogging::Logger::Helper((n), QsLogging::ErrorLevel).stream()
#define QLOG_FATAL(n) \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::FatalLevel ){} \
    else QsLogging::Logger::Helper((n), QsLogging::FatalLevel).stream()
#else
#ifdef NDEBUG
#define QLOG_TRACE(n)    if (true) {} else QsLogging::Logger::Helper((n), QsLogging::TraceLevel).stream()
#define QLOG_DEBUG(n)    if (true) {} else QsLogging::Logger::Helper((n), QsLogging::DebugLevel).stream()
#else
#define QLOG_TRACE(n) \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::TraceLevel ){} \
    else  QsLogging::Logger::Helper((n), QsLogging::TraceLevel).stream() << qPrintable(QString("%1:%2 - ").arg(__FILE__).arg(__LINE__))
#define QLOG_DEBUG(n) \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::DebugLevel ){} \
    else QsLogging::Logger::Helper((n), QsLogging::DebugLevel).stream() << qPrintable(QString("%1:%2 - ").arg(__FILE__).arg(__LINE__))
#endif
#define QLOG_INFO(n)  \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::InfoLevel ){} \
    else QsLogging::Logger::Helper((n), QsLogging::InfoLevel).stream() << qPrintable(QString("%1:%2 - ").arg(__FILE__).arg(__LINE__))
#define QLOG_WARN(n)  \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::WarnLevel ){} \
    else QsLogging::Logger::Helper((n), QsLogging::WarnLevel).stream() << qPrintable(QString("%1:%2 - ").arg(__FILE__).arg(__LINE__))
#define QLOG_ERROR(n) \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::ErrorLevel ){} \
    else QsLogging::Logger::Helper((n), QsLogging::ErrorLevel).stream() << qPrintable(QString("%1:%2 - ").arg(__FILE__).arg(__LINE__))
#define QLOG_FATAL(n) \
    if( QsLogging::Logger::instance((n)).loggingLevel() > QsLogging::FatalLevel ){} \
    else QsLogging::Logger::Helper((n), QsLogging::FatalLevel).stream() << qPrintable(QString("%1:%2 - ").arg(__FILE__).arg(__LINE__))
#endif

#ifdef QS_LOG_DISABLE
#include "QsLogDisableForThisFile.h"
#endif

#endif // QSLOG_H
