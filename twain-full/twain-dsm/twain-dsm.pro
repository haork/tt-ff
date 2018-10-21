#-------------------------------------------------
#
# Project created by QtCreator 2018-09-08T10:30:02
#
#-------------------------------------------------

QT -= gui

TARGET = twaindsm
TEMPLATE = lib
CONFIG += staticlib debug_and_release
DESTDIR = ../lib

SOURCES += \
    apps.cpp \
    dsm.cpp \
    hook.cpp \
    log.cpp

HEADERS += \
    dsm.h \
    dsm.rc \
    resource.h \
    twain.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    dsm.def \
    DSM_Translations.txt

DEFINES -= UNICODE
