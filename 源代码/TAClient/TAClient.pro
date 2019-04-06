#-------------------------------------------------
#
# Project created by QtCreator 2018-04-03T17:25:08
#
#-------------------------------------------------

QT       += core gui network sql

TARGET = TAClient
TEMPLATE = app
DEPENDPATH += . \
              control \
              model \
              view \
              resource/image

SOURCES += main.cpp\
        taloginwidget.cpp \
    taclicklabel.cpp \
    model/taencryption.cpp \
    control/taloginctl.cpp \
    model/tatcpsocket.cpp \
    tamainwidget.cpp \
    taregisterwidget.cpp \
    control/taclientfilectl.cpp \
    control/tamainctl.cpp \
    control/tafilectl.cpp \
    tachatwidget.cpp \
    taflockwidget.cpp \
    tatoolitem.cpp \
    tafriendbutton.cpp \
    control/taregisterctl.cpp \
    taflockbutton.cpp \
    tainformationwidget.cpp \
    taflockinformationwidget.cpp \
    control/tadatabasectl.cpp \
    tafriendlistwidget.cpp \
    tasearchlistwidget.cpp \
    taflocklistwidget.cpp \
    talatestmessagelistwidget.cpp \
    talinkbutton.cpp \
    talinkfriendbutton.cpp \
    talinkflockbutton.cpp \
    taflockmemberlistwidget.cpp \
    taflockmemberbutton.cpp \
    model/tasendudpsocket.cpp \
    model/tareiverudpsockeet.cpp

HEADERS  += taloginwidget.h \
    taclicklabel.h \
    model/taencryption.h \
    control/taloginctl.h \
    model/taconstant.h \
    model/tatcpsocket.h \
    tamainwidget.h \
    taregisterwidget.h \
    control/taregisterctl.h \
    control/taclientfilectl.h \
    control/tamainctl.h \
    control/tafilectl.h \
    tachatwidget.h \
    taflockwidget.h \
    tatoolitem.h \
    tafriendbutton.h \
    taflockbutton.h \
    tainformationwidget.h \
    taflockinformationwidget.h \
    control/tadatabasectl.h \
    tafriendlistwidget.h \
    tasearchlistwidget.h \
    taflocklistwidget.h \
    talatestmessagelistwidget.h \
    talinkbutton.h \
    talinkfriendbutton.h \
    talinkflockbutton.h \
    taflockmemberlistwidget.h \
    taflockmemberbutton.h \
    model/tasendudpsocket.h \
    model/tareiverudpsockeet.h

FORMS    += taloginwidget.ui \
    tamainwidget.ui \
    taregisterwidget.ui \
    tachatwidget.ui \
    taflockwidget.ui \
    tainformationwidget.ui \
    taflockinformationwidget.ui

RESOURCES += \
    imgRes.qrc

OTHER_FILES +=
