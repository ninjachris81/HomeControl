#ifndef CURRENTCONTROLLER_H
#define CURRENTCONTROLLER_H

#include <QObject>
#include "controllerbase.h"
#include <QLoggingCategory>

//#include "utils/rpianalogreader.h"

Q_DECLARE_LOGGING_CATEGORY(LG_CURRENT_CONTROLLER)

//#define CURRENT_UPDATE_VALUE_INTERVAL 5000

//#define RPI_CURRENT_ADC 1
//#define RPI_CURRENT_GAIN 0

class CurrentController : public ControllerBase
{
    Q_OBJECT
public:
    explicit CurrentController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return CURRENT_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QString getEnumName();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    static QString CONTROLLER_NAME;

private:
    //RPIAnalogReader analogReader;

protected:
    void onInit();

    void onMqttConnected();

    void onValueChanged(int index, QVariant value);

    //void onScheduleUpdate();


signals:

public slots:

};

#endif // CURRENTCONTROLLER_H
