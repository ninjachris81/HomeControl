#include "zerologic.h"
#include <QDebug>

#include "controller/brightnesscontroller.h"

Q_LOGGING_CATEGORY(LG_ZERO_LOGIC, "ZeroLogic");

ZeroLogic::ZeroLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, ZERO_LOGIC_INTERVAL, parent)
{
}

void ZeroLogic::onMaintenance() {
    qCDebug(LG_ZERO_LOGIC) << Q_FUNC_INFO;
}

void ZeroLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qCDebug(LG_ZERO_LOGIC) << Q_FUNC_INFO << cmd;
}
