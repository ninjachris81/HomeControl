# HANNspree HT161HNB 39,6cm (15,6") 1366x768 on RPI3

QT += quick
QT += mqtt virtualkeyboard
QT += sql
QT += charts
QT += serialport
#QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    databridge.cpp \
    screensaverfilter.cpp

RESOURCES += qml.qrc \
    images.qrc \
    styledef.qrc \
    translations.qrc

TRANSLATIONS = languages/hc.en.ts  languages/hc.de.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH += qml
QML_IMPORT_PATH += $$PWD/../SharedLib/qml

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

HEADERS += \
    constant_strings.h \
    databridge.h \
    screensaverfilter.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = $$[QT_INSTALL_PREFIX]/opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-SharedLib-Desktop_Qt_5_12_0_MSVC2017_64bit2-Debug/release/ -lSharedLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-SharedLib-Desktop_Qt_5_12_0_MSVC2017_64bit2-Debug/debug/ -lSharedLib
else:unix: LIBS += -L$$[QT_INSTALL_LIBS] -lSharedLib

INCLUDEPATH += $$PWD/../SharedLib/include
DEPENDPATH += $$PWD/../SharedLib

system(perl ..\SharedLib\utils\generate_ts.pl ..\SharedLib\include\constants.h $$PWD\constant_strings.h)
