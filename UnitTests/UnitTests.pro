QT += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = UnitTests
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp


win32: LIBS += -L$$PWD/gtest/ -lgmock

INCLUDEPATH += $$PWD/gtest/include
DEPENDPATH += $$PWD/gtest/include

PRE_TARGETDEPS += $$PWD/gtest/libgmock.a
