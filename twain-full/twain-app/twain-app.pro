#-------------------------------------------------
#
# Project created by QtCreator 2018-09-08T10:22:16
#
#-------------------------------------------------

QT -= gui

TARGET = twainapp
TEMPLATE = lib
CONFIG += staticlib debug_and_release
DESTDIR = ../lib

INCLUDEPATH +=
LIBS += -L../lib \
    -lfreeimage \
    -ltwaindsm

SOURCES += twainapp.cpp \
    CommonTWAIN.cpp \
    CTiffWriter.cpp \
    TwainString.cpp \
    DSMInterface.cpp \
    main.cpp

HEADERS += twainapp.h \
    FreeImage.h \
    Common.h \
    CommonTWAIN.h \
    CTiffWriter.h \
    TwainString.h \
    DSMInterface.h
unix {
    target.path = /usr/lib
    INSTALLS += target
    DEFINES += UNIX \
        kTWAIN_DS_DIR=\"/usr/local/lib/twain\"
}
