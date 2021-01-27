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
    tcp/TCPClient.cpp \
    tcp/TCPServer.cpp \
    tcp/TCPPacket.cpp \
    config/Config.cpp \
    tcp/TCPConnection.cpp

HEADERS += \
    contacts/contact_validator.h \
    contacts/Contact.h \
    contacts/Storage.h \
    contacts/Message.h \
    tcp/TCPClient.h \
    tcp/TCPServer.h \
    tcp/TCPPacket.h \
    tcp/TCPException.h \
    util/strutil.h \
    config/Config.h \
    IOException.h \
    util/strbuilder.h \
    util/util.h \
    tcp/TCPConnection.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
