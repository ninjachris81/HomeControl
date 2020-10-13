#ifndef MOTIONDETECTORLOGIC_H
#define MOTIONDETECTORLOGIC_H

#include <QObject>
#include <QLoggingCategory>
#include "controller/logiccontroller.h"

#include "utils/rpigpiomanager.h"

#define MOTION_DETECTOR_LOGIC_INTERVAL 1000

#define BRIGHTNESS_THRESHOLD 10000

Q_DECLARE_LOGGING_CATEGORY(LG_MOTION_DETECTOR_LOGIC)

class MotionDetectorLogic : public LogicController
{
    Q_OBJECT
public:
    explicit MotionDetectorLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

private:
    RPIGpioManager gpioManager;

signals:

public slots:
    void onMaintenance();

    void onCommandReceived(EnumsDeclarations::MQTT_CMDS cmd);
};

#endif // MOTIONDETECTORLOGIC_H
