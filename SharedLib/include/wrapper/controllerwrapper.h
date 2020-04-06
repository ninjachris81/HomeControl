#ifndef CONTROLLERWRAPPER_H
#define CONTROLLERWRAPPER_H

#include <QObject>
#include "controller/controllerbase.h"
#include "constants_qt.h"
#include <QQmlEngine>

class ControllerWrapper : public QObject {
    //Q_OBJECT
public:
    ControllerWrapper(ControllerBase* controller);

    Q_INVOKABLE void setValue(int index, QVariant value);

    Q_INVOKABLE void sendCmd(EnumsDeclarations::MQTT_CMDS cmd);

    static void registerTypes() {
        qmlRegisterUncreatableMetaObject(EnumsDeclarations::staticMetaObject, "hc", 1, 0, "Enums", "Access to enums & flags only");
        qRegisterMetaType<EnumsDeclarations::MQTT_CMDS>("EnumsDeclarations::MQTT_CMDS");
    }

protected:
    ControllerBase* m_controller;
};

#endif // CONTROLLERWRAPPER_H
