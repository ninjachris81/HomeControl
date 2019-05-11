#-------------------------------------------------
#
# Project created by QtCreator 2019-04-05T20:27:58
#
#-------------------------------------------------

QT += qml
QT += mqtt
QT += sql

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
        source/controller/relaycontroller.cpp \
        source/controller/tempcontroller.cpp \
        source/controller/logiccontroller.cpp \
        source/controller/settingscontroller.cpp \
        source/listmodel/controllerlistmodel.cpp \
        source/listmodel/relaycontrollerlistmodel.cpp \
        source/listmodel/settingscontrollerlistmodel.cpp \
        source/wrapper/controllerwrapper.cpp \
    source/wrapper/settingswrapper.cpp \
    source/controller/logcontroller.cpp \
    source/listmodel/logcontrollerlistmodel.cpp \
    source/sqllistmodel/logsqllistmodel.cpp \
    source/utils/qsqldatabasewrapper.cpp \
    source/sqllistmodel/sqlquerymodel.cpp \
    source/wrapper/logwrapper.cpp

HEADERS += \
        include/sharedlib.h \
        include/appconfiguration.h \
        include/constants.h \
        include/constants_qt.h \
        include/controller/controllerbase.h \
        include/controller/controllermanager.h \
        include/controller/relaycontroller.h \
        include/controller/tempcontroller.h \
        include/controller/settingscontroller.h \
        include/controller/logiccontroller.h \
        include/listmodel/controllerlistmodel.h \
        include/listmodel/relaycontrollerlistmodel.h \
        include/listmodel/settingscontrollerlistmodel.h \
        include/wrapper/controllerwrapper.h \
    include/wrapper/settingswrapper.h \
    include/controller/logcontroller.h \
    include/listmodel/logcontrollerlistmodel.h \
    include/sqllistmodel/logsqllistmodel.h \
    include/utils/qsqldatabasewrapper.h \
    include/sqllistmodel/sqlquerymodel.h \
    include/wrapper/logwrapper.h

unix {
    target.path = $$[QT_INSTALL_LIBS]
    INSTALLS += target
}
