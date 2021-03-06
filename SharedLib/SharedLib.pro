#-------------------------------------------------
#
# Project created by QtCreator 2019-04-05T20:27:58
#
#-------------------------------------------------

QT += qml
QT += mqtt
QT += sql
QT += quick
QT += charts
QT += serialport

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
    source/controller/ssccontroller.cpp \
    source/controller/currentcontroller.cpp \
    source/controller/eventcontroller.cpp \
    source/controller/humiditycontroller.cpp \
    source/controller/powercontroller.cpp \
    source/controller/pvcontroller.cpp \
    source/sqllistmodel/hcsqlquerymodel.cpp \
    source/utils/databasemanager.cpp \
    source/controller/dataloggercontroller.cpp \
    source/sqllistmodel/dataloggersqllistmodel.cpp \
    source/utils/databridgebase.cpp \
    source/utils/dht_utils.cpp \
    source/utils/fontloader.cpp \
    source/utils/apptranslator.cpp \
    source/utils/rpianalogreader.cpp \
    source/utils/rpigpiomanager.cpp \
    source/utils/serialportreader.cpp \
    source/utils/weatherforecast.cpp \
    source/utils/weatherforecastmanager.cpp \
    source/utils/wiringpimanager.cpp \
    source/wrapper/dataloggerwrapper.cpp \
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
    source/wrapper/logwrapper.cpp \
    source/controller/brightnesscontroller.cpp \
    source/utils/forecastutils.cpp

HEADERS += \
    include/config/rpi_gpio_zero.h \
    include/controller/currentcontroller.h \
    include/controller/dataloggercontroller.h \
    include/controller/humiditycontroller.h \
    include/controller/infocontroller.h \
    include/controller/powercontroller.h \
    include/controller/pvcontroller.h \
    include/controller/ssccontroller.h \
    include/controller/switchcontroller.h \
    include/sqllistmodel/dataloggersqllistmodel.h \
        include/sharedlib.h \
    include/sqllistmodel/hcsqlquerymodel.h \
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
    include/utils/databasemanager.h \
    include/utils/databridgebase.h \
    include/utils/fontloader.h \
    include/utils/rpianalogreader.h \
    include/utils/rpigpiomanager.h \
    include/utils/serialportreader.h \
    include/utils/version.h \
    include/utils/weatherforecast.h \
    include/utils/weatherforecastmanager.h \
    include/utils/wiringpimanager.h \
        include/wrapper/controllerwrapper.h \
    include/wrapper/dataloggerwrapper.h \
    include/wrapper/infowrapper.h \
    include/wrapper/settingswrapper.h \
    include/controller/logcontroller.h \
    include/sqllistmodel/logsqllistmodel.h \
    include/utils/qsqldatabasewrapper.h \
    include/wrapper/logwrapper.h \
    include/controller/brightnesscontroller.h \
    include/utils/apptranslator.h \
    include/utils/forecastutils.h \
    include/controller/eventcontroller.h

unix {
    target.path = $$[QT_INSTALL_LIBS]
    INSTALLS += target
}

message(Checking WiringPi)
equals(USE_WIRING_PI, 1) {
    message(Using wiring pi)
    DEFINES += USE_WIRING_PI
    LIBS += -I/usr/local/include -L/usr/local/lib -lwiringPi
} else {
    message(Not using wiringPi)
}

INCLUDEPATH += $$PWD/include

RESOURCES += \
    sharedcomponents.qrc \
    sharedfonts.qrc \
    sharedimages.qrc \
    sharedutils.qrc

DISTFILES += \
    utils/build_inc.bat \
    utils/generate_ts.pl
