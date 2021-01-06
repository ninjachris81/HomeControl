#include "powerlogic.h"

#include "controller/pvcontroller.h"
#include "controller/currentcontroller.h"

#include "constants_qt.h"

Q_LOGGING_CATEGORY(LG_POWER_LOGIC, "PowerLogic");

PowerLogic::PowerLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, POWER_LOGIC_INTERVAL, parent)
{
    CurrentController* currentController = static_cast<CurrentController*>(controllerManager->getController(CurrentController::CONTROLLER_NAME));
    m_powerController = static_cast<PowerController*>(controllerManager->getController(PowerController::CONTROLLER_NAME));
    connect(currentController, &CurrentController::valueChanged, this, &PowerLogic::onCurrentValueChanged);
}

void PowerLogic::onCurrentValueChanged(int index, QVariant value) {
    if (index==EnumsDeclarations::CURRENTS_PV) {
        // publish power
        double pvPower = value.toInt() * 0.230;

        qCDebug(LG_POWER_LOGIC) << Q_FUNC_INFO << pvPower;

        m_powerController->setValue(EnumsDeclarations::POWERS_PV, pvPower);
        m_powerController->publishValue(EnumsDeclarations::POWERS_PV);
    }
}
