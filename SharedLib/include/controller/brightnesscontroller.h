#ifndef BRIGHTNESSCONTROLLER_H
#define BRIGHTNESSCONTROLLER_H

#include <QObject>
#include "controllerbase.h"
#include <QLoggingCategory>

#include "utils/rpianalogreader.h"

Q_DECLARE_LOGGING_CATEGORY(LG_BRIGHTNESS_CONTROLLER)

#define BRIGHTNESS_UPDATE_VALUE_INTERVAL 5000

#define RPI_BRIGHTNESS_ADC 0
#define RPI_BRIGHTNESS_GAIN 0

class BrightnessController : public ControllerBase
{
    Q_OBJECT
public:
    explicit BrightnessController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return BRIGHTNESS_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QString getEnumName();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    static QString CONTROLLER_NAME;

private:
    RPIAnalogReader analogReader;

protected:
    void onInit();

    void onMqttConnected();

    void onValueChanged(int index, QVariant value);

    void onScheduleUpdate();

signals:

public slots:

};

#endif // BRIGHTNESSCONTROLLER_H
