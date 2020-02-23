#-------------------------------------------------
#
# Project created by QtCreator 2019-04-05T20:27:58
#
#-------------------------------------------------

QT += qml
QT += mqtt
QT += sql
QT += quick

#QT       -= gui

TARGET = SharedLib
TEMPLATE = lib
CONFIG += staticlib

CONFIG += c++11

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
    source/utils/apptranslator.cpp \
    source/utils/weatherforecast.cpp \
    source/utils/weatherforecastmanager.cpp \
    source/wrapper/infowrapper.cpp \
    source/controller/infocontroller.cpp \
    source/controller/switchcontroller.cpp \
        source/sharedlib.cpp \
        source/utils/appconfiguration.cpp \
        source/controller/controllerbase.cpp \
        source/controller/controllermanager.cpp \
        source/controller/relaycontroller.cpp \
        source/controller/tempcontroller.cpp \
        source/controller/logiccontroller.cpp \
        source/controller/settingscontroller.cpp \
        source/listmodel/controllerlistmodel.cpp \
        source/wrapper/controllerwrapper.cpp \
    source/wrapper/settingswrapper.cpp \
    source/controller/logcontroller.cpp \
    source/sqllistmodel/logsqllistmodel.cpp \
    source/utils/qsqldatabasewrapper.cpp \
    source/sqllistmodel/sqlquerymodel.cpp \
    source/wrapper/logwrapper.cpp \
    source/controller/brightnesscontroller.cpp

HEADERS += \
    include/controller/infocontroller.h \
    include/controller/switchcontroller.h \
        include/sharedlib.h \
        include/utils/appconfiguration.h \
        include/constants.h \
        include/constants_qt.h \
        include/controller/controllerbase.h \
        include/controller/controllermanager.h \
        include/controller/relaycontroller.h \
        include/controller/tempcontroller.h \
        include/controller/settingscontroller.h \
        include/controller/logiccontroller.h \
        include/listmodel/controllerlistmodel.h \
    include/utils/weatherforecast.h \
    include/utils/weatherforecastmanager.h \
        include/wrapper/controllerwrapper.h \
    include/wrapper/infowrapper.h \
    include/wrapper/settingswrapper.h \
    include/controller/logcontroller.h \
    include/sqllistmodel/logsqllistmodel.h \
    include/utils/qsqldatabasewrapper.h \
    include/sqllistmodel/sqlquerymodel.h \
    include/wrapper/logwrapper.h \
    include/controller/brightnesscontroller.h \
    include/utils/apptranslator.h

unix {
    target.path = $$[QT_INSTALL_LIBS]
    INSTALLS += target
}

INCLUDEPATH += $$PWD/include

RESOURCES += \
    sharedcomponents.qrc

DISTFILES += \
    utils/generate_ts.pl
