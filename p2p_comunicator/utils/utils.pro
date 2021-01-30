QT -= gui
QT += network

TEMPLATE = lib
DEFINES += UTILS_LIBRARY

CONFIG += c++11 staticlib

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    contacts/contact_validator.cpp \
    contacts/Contact.cpp \
    contacts/Storage.cpp \
    contacts/Message.cpp \
    config/Config.cpp \
    contacts/ContactController.cpp

HEADERS += \
    contacts/contact_validator.h \
    contacts/Contact.h \
    contacts/Storage.h \
    contacts/Message.h \
    util/strutil.h \
    config/Config.h \
    IOException.h \
    util/strbuilder.h \
    util/util.h \
    contacts/ContactController.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../tcp/release/ -ltcp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../tcp/debug/ -ltcp
else:unix: LIBS += -L$$OUT_PWD/../tcp/ -ltcp

INCLUDEPATH += $$PWD/../tcp
DEPENDPATH += $$PWD/../tcp

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tcp/release/libtcp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tcp/debug/libtcp.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tcp/release/tcp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../tcp/debug/tcp.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../tcp/libtcp.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../logger/release/ -llogger
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../logger/debug/ -llogger
else:unix: LIBS += -L$$OUT_PWD/../logger/ -llogger

INCLUDEPATH += $$PWD/../logger
DEPENDPATH += $$PWD/../logger

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logger/release/liblogger.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logger/debug/liblogger.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logger/release/logger.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../logger/debug/logger.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../logger/liblogger.a
