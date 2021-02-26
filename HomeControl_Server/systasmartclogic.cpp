#include "systasmartclogic.h"

Q_LOGGING_CATEGORY(LG_SYSTA_SMART_C_LOGIC, "SSC Logic")

SystaSmartCLogic::SystaSmartCLogic(ControllerManager *controllerManager, SystaSmartCClient *client, AppConfiguration &appConfig, QObject *parent) : LogicController(controllerManager, 0, parent), m_client(client)
{
    m_sscController = static_cast<SSCController*>(controllerManager->getController(SSCController::CONTROLLER_NAME));

    client->setupVariableRequest(EnumsDeclarations::SSC_STATUS_HK1, 5000);
    client->setupVariableRequest(EnumsDeclarations::SSC_STATUS_BOILER, 5000);

    connect(client, &SystaSmartCClient::receivedVariable, this, &SystaSmartCLogic::onReceivedVariable);
}

void SystaSmartCLogic::onMaintenance() {
}

void SystaSmartCLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qCDebug(LG_SYSTA_SMART_C_LOGIC) << Q_FUNC_INFO << cmd;

    //switch(cmd) {
    //case EnumsDeclarations::START_PREHEAT:
    //  break;
    //}
}

void SystaSmartCLogic::onReceivedVariable(EnumsDeclarations::MQTT_SSC_VARIABLES variable, QVariant value) {
    qCDebug(LG_SYSTA_SMART_C_LOGIC) << Q_FUNC_INFO << variable << value;

    if (m_sscController->setValue(variable, value)) {
        m_sscController->publishValue(variable);
    }
}
