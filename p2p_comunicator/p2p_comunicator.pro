TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    logger \
    tcp \
    utils \
    tests \
    app

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/logger/release/ -llogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/logger/debug/ -llogger
else:unix: LIBS += -L$$OUT_PWD/logger/ -llogger

INCLUDEPATH += $$PWD/logger
DEPENDPATH += $$PWD/logger

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/logger/release/liblogger.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/logger/debug/liblogger.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/logger/release/logger.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/logger/debug/logger.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/logger/liblogger.a
