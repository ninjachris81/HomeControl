#include "motiondetectorlogic.h"

#include "controller/brightnesscontroller.h"
#include "controller/relaycontroller.h"
#include "controller/switchcontroller.h"
#include "controller/settingscontroller.h"


Q_LOGGING_CATEGORY(LG_MOTION_DETECTOR_LOGIC, "MotionDetectorLogic");

MotionDetectorLogic::MotionDetectorLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, MOTION_DETECTOR_LOGIC_INTERVAL, parent)
{
}

void MotionDetectorLogic::onMaintenance() {
    qCDebug(LG_MOTION_DETECTOR_LOGIC) << Q_FUNC_INFO;

    int brightnessThreshold = m_controllerManager->getController(SettingsController::CONTROLLER_NAME)->value(EnumsDeclarations::SETTINGS_MOTION_SENSOR_BRIGHTNESS_THRESHOLD).toInt();

    int brightnessOutside = m_controllerManager->getController(BrightnessController::CONTROLLER_NAME)->value(EnumsDeclarations::BRIGHTNESSES_OUTSIDE).toInt();
    bool lightOutsideOnNew = false;

    if (brightnessOutside<brightnessThreshold) {
        bool pirOn = m_controllerManager->getController(SwitchController::CONTROLLER_NAME)->value(EnumsDeclarations::SWITCHES_PIR).toBool();

        qCDebug(LG_MOTION_DETECTOR_LOGIC) << "PIR" << pirOn;

        lightOutsideOnNew = pirOn;
    } else {
        // too bright
    }

    if (firstRun || lightOutsideOnNew!=lightOutsideOn) {
        lightOutsideOn = lightOutsideOnNew;
        m_controllerManager->getController(RelayController::CONTROLLER_NAME)->setValue(EnumsDeclarations::RELAYS_LIGHT_OUTSIDE, lightOutsideOn);
        m_controllerManager->getController(RelayController::CONTROLLER_NAME)->publishValue(EnumsDeclarations::RELAYS_LIGHT_OUTSIDE);
        firstRun = false;
    }
}

void MotionDetectorLogic::onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd) {
    qCDebug(LG_MOTION_DETECTOR_LOGIC) << Q_FUNC_INFO << cmd;
}
