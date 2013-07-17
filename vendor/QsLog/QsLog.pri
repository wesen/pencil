INCLUDEPATH += \"vendor/QsLog\"
#DEFINES += QS_LOG_LINE_NUMBERS
#DEFINES += QS_LOG_DISABLE
SOURCES += vendor/QsLog/QsLogDest.cpp \
    vendor/QsLog/QsLog.cpp \
    vendor/QsLog/QsDebugOutput.cpp

HEADERS += vendor/QsLog/QSLogDest.h \
    vendor/QsLog/QsLog.h \
    vendor/QsLog/QsDebugOutput.h \
    vendor/QsLog/QsLogLevel.h \
    vendor/QsLog/QsLogDisableForThisFile.h
