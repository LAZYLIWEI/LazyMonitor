QT += multimedia widgets network

CONFIG += C++11

HEADERS += \
    MonitorSocket.h \
    MonitorConfig.h \
    MonitorCommand.h \
    MonitorUtil.h \
    MonitorCamera.h

SOURCES += \
    main.cpp \
    MonitorSocket.cpp \
    MonitorConfig.cpp \
    MonitorUtil.cpp \
    MonitorCamera.cpp

LIBS += ""
