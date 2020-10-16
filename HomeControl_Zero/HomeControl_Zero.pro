QT -= gui
QT += mqtt
QT += sql

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    motiondetectorlogic.cpp \
    zerologic.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    motiondetectorlogic.h \
    zerologic.h


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-SharedLib-Desktop_Qt_5_12_0_MSVC2017_64bit2-Debug/release/ -lSharedLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-SharedLib-Desktop_Qt_5_12_0_MSVC2017_64bit2-Debug/debug/ -lSharedLib
else:unix: LIBS += -L$$PWD/../build-SharedLib-Desktop_Qt_5_12_0_MSVC2017_64bit2-Debug/ -lSharedLib

message(Checking WiringPi)
equals(USE_WIRING_PI, 1) {
    message(Using wiring pi)
    DEFINES += USE_WIRING_PI
    LIBS += -I/usr/local/include -L/usr/local/lib -lwiringPi
} else {
    message(Not using wiringPi)
}


INCLUDEPATH += $$PWD/../SharedLib/include
DEPENDPATH += $$PWD/../SharedLib
