#-------------------------------------------------
#
# Project created by QtCreator 2018-09-07T22:31:12
#
#-------------------------------------------------

QT       += network sql
QT       -= gui

CONFIG   += debug

INCLUDEPATH += ../qhttpserver \
            ../twain-app
LIBS += -L../lib \
        -ltwainapp \
        -lfreeimage

unix {
    LIBS +=-ldl
}

win32 {
    debug: LIBS += -lqhttpserverd \
                   -lUser32
    else: LIBS += -lqhttpserver \
                  -lUser32
} else {
    LIBS += -lqhttpserver
}

TARGET = twain-server
TEMPLATE = app


SOURCES +=\
    requesthandler.cpp \
    main.cpp \
    mytwainapp.cpp \
    responder.cpp \
    twainresponder.cpp

HEADERS  += \
    requesthandler.h \
    mytwainapp.h \
    responder.h \
    twainresponder.h

DISTFILES += \
    config.ini

RESOURCES += \
    resources.qrc
