QT += core sql
QT -= gui

CONFIG += c++11

TARGET = ExampleDataGenerator
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../TaskBoard

SOURCES += main.cpp \
    ../TaskBoard/databasemanager.cpp \
    datagenerator.cpp \
    ../TaskBoard/projectentry.cpp \
    ../TaskBoard/taskentry.cpp

HEADERS += \
    ../TaskBoard/databasemanager.h \
    datagenerator.h \
    ../TaskBoard/projectentry.h \
    ../TaskBoard/taskentry.h

