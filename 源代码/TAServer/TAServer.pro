#-------------------------------------------------
#
# Project created by QtCreator 2018-04-13T13:03:04
#
#-------------------------------------------------

QT       += core gui network sql

TARGET = TAServer
TEMPLATE = app
LIBS    += -L/path/to/library -leventdispatcher_epoll

SOURCES += main.cpp\
        taserverwindow.cpp \
    model/tauser.cpp \
    model/tathread.cpp \
    control/taservermessagectl.cpp \
    control/tadatabasectl.cpp \
    control/taclientsocketctl.cpp \
    model/tatcpserver.cpp \
    model/tahandlemessage.cpp

HEADERS  += taserverwindow.h /path/to/eventdispatcher_epoll.h \
    model/tauser.h \
    model/tathread.h \
    control/taservermessagectl.h \
    control/tadatabasectl.h \
    control/taclientsocketctl.h \
    model/tatcpserver.h \
    model/TAConstant.h \
    model/TAEncryption.h \
    model/tahandlemessage.h

FORMS    += taserverwindow.ui
