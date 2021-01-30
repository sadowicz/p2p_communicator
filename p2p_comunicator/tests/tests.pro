TEMPLATE = subdirs

SUBDIRS += \
    contact_test \
    contact_validator \
    storage_test \
    tcp_packet_test

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../utils/release/ -lutils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../utils/debug/ -lutils
else:unix: LIBS += -L$$OUT_PWD/../utils/ -lutils

INCLUDEPATH += $$PWD/../utils
DEPENDPATH += $$PWD/../utils

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
