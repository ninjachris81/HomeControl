#-------------------------------------------------
#
# Project created by QtCreator 2019-04-05T20:27:58
#
#-------------------------------------------------

QT += qml
QT += mqtt

QT       -= gui

TARGET = SharedLib
TEMPLATE = lib
CONFIG += staticlib

DEFINES += SHAREDLIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        source/sharedlib.cpp \
        source/appconfiguration.cpp \
        source/controller/controllerbase.cpp \
        source/controller/controllermanager.cpp \
        source/controller/errorcontroller.cpp \
        source/controller/relaycontroller.cpp \
        source/controller/tempcontroller.cpp \
        source/controller/logiccontroller.cpp \
        source/controller/settingscontroller.cpp \
        source/listmodel/controllerlistmodel.cpp \
        source/listmodel/errorcontrollerlistmodel.cpp \
        source/listmodel/relaycontrollerlistmodel.cpp \
        source/listmodel/settingscontrollerlistmodel.cpp \
        source/wrapper/controllerwrapper.cpp \
    source/wrapper/settingswrapper.cpp

HEADERS += \
        include/sharedlib.h \
        include/appconfiguration.h \
        include/constants.h \
        include/constants_qt.h \
        include/controller/controllerbase.h \
        include/controller/controllermanager.h \
        include/controller/errorcontroller.h \
        include/controller/relaycontroller.h \
        include/controller/tempcontroller.h \
        include/controller/settingscontroller.h \
        include/controller/logiccontroller.h \
        include/listmodel/controllerlistmodel.h \
        include/listmodel/errorcontrollerlistmodel.h \
        include/listmodel/relaycontrollerlistmodel.h \
        include/listmodel/settingscontrollerlistmodel.h \
        include/wrapper/controllerwrapper.h \
    include/wrapper/settingswrapper.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
