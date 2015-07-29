#-------------------------------------------------
#
# Project created by QtCreator 2015-05-27T06:27:37
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = InDivProvServer
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
        indiprov.cpp\
        prov-core.cxx\
        xhtml-datatypes-1.cxx\
        xml.cxx \
        provc.cpp \
        provutils.cpp

HEADERS  += mainwindow.h \
            indiprov.h \
            prov-core.hxx\
            xhtml-datatypes-1.hxx\
            xml.hxx \
            provc.h \
            provutils.h

FORMS    += mainwindow.ui

LIBS+= -lzmq -lmysqlcppconn -lxerces-c -lboost_thread -lboost_system  \
        -lboost_date_time\
        -lboost_filesystem \
        -lboost_program_options \
        -lboost_regex \
        -ljson
