QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Device_Controller.cpp \
    client.cpp \
    client_commands.cpp \
    clients_manager.cpp \
    main.cpp \
    client_window.cpp

HEADERS += \
    Device_Controller.h \
    client.h \
    client_commands.h \
    client_window.h \
    clients_manager.h

FORMS += \
    client_commands.ui \
    client_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
