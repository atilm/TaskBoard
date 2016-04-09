#-------------------------------------------------
#
# Project created by QtCreator 2016-03-09T12:04:00
#
#-------------------------------------------------

QT       += core gui sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = appIcon.ico

TARGET = TaskBoard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    taskcolumn.cpp \
    taskmodel.cpp \
    databasemanager.cpp \
    qlistviewdelegate.cpp \
    taskentry.cpp \
    edittaskdialog.cpp \
    colorcontainer.cpp \
    editprojectdialog.cpp \
    projectentry.cpp

HEADERS  += mainwindow.h \
    taskcolumn.h \
    taskmodel.h \
    databasemanager.h \
    qlistviewdelegate.h \
    taskentry.h \
    edittaskdialog.h \
    colorcontainer.h \
    editprojectdialog.h \
    projectentry.h

FORMS    += mainwindow.ui \
    taskcolumn.ui \
    edittaskdialog.ui \
    editprojectdialog.ui

RESOURCES += \
    resources.qrc
