#include "motiondetectorlogic.h"

#include "controller/brightnesscontroller.h"
#include "controller/relaycontroller.h"
#include "controller/switchcontroller.h"


Q_LOGGING_CATEGORY(LG_MOTION_DETECTOR_LOGIC, "MotionDetectorLogic");

MotionDetectorLogic::MotionDetectorLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, MOTION_DETECTOR_LOGIC_INTERVAL, parent)
{
}

void MotionDetectorLogic::onMaintenance() {
    qCDebug(LG_MOTION_DETECTOR_LOGIC) << Q_FUNC_INFO;

    int brightnessOutside = m_controllerManager->getController(BrightnessController::CONTROLLER_NAME)->value(MQTT_PATH_BRIGHTNESSES_OUTSIDE).toInt();
    bool lightOutsideOn = false;

    if (brightnessOutside<BRIGHTNESS_THRESHOLD) {
        bool pirOn = m_controllerManager->getController(SwitchController::CONTROLLER_NAME)->value(MQTT_PATH_SWITCHES_PIR).toBool();

        qCDebug(LG_MOTION_DETECTOR_LOGIC) << "PIR" << pirOn;

        lightOutsideOn = pirOn;
    } else {
        // too bright
    }

    m_controllerManager->getController(RelayController::CONTROLLER_NAME)->setValue(MQTT_PATH_RELAYS_LIGHT_OUTSIDE, lightOutsideOn, true);
}

void MotionDetectorLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qCDebug(LG_MOTION_DETECTOR_LOGIC) << Q_FUNC_INFO << cmd;
}
