#-------------------------------------------------
#
# Project created by QtCreator 2023-02-24T18:05:44
#
#-------------------------------------------------

QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyQQ
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES +=\
        main.cpp \
        loginview.cpp \
    mainchat.cpp \
    logonview.cpp \
    tcpsocketclient.cpp \
    friendmaking.cpp \
    myproto.cpp \
    jsoncpp.cpp \
    frienditem.cpp \
    verifymsg.cpp \
    verifymsgitem.cpp \
    chatting.cpp \
    content.cpp

HEADERS += loginview.h \
    mainchat.h \
    logonview.h \
    tcpsocketclient.h \
    friendmaking.h \
    myproto.h \
    json/json.h \
    frienditem.h \
    verifymsg.h \
    verifymsgitem.h \
    storageclass.hpp \
    chatting.h \
    content.h

FORMS += loginview.ui \
    mainchat.ui \
    logonview.ui \
    friendmaking.ui \
    frienditem.ui \
    verifymsg.ui \
    verifymsgitem.ui \
    chatting.ui \
    content.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
