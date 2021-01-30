QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    add_contact_window.cpp \
    editcontactwindow.cpp \
    error_window.cpp \
    main.cpp \
    mainwindow.cpp \
    settingswindow.cpp

HEADERS += \
    add_contact_window.h \
    editcontactwindow.h \
    error_window.h \
    mainwindow.h \
    settingswindow.h

FORMS += \
    add_contact_window.ui \
    editcontactwindow.ui \
    error_window.ui \
    mainwindow.ui \
    settingswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../utils/release/ -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../utils/debug/ -lutils
else:unix: LIBS += -L$$OUT_PWD/../utils/ -lutils

INCLUDEPATH += $$PWD/../utils
DEPENDPATH += $$PWD/../utils

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
