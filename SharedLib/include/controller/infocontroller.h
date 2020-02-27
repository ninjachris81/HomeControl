#ifndef INFOCONTROLLER_H
#define INFOCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QLoggingCategory>
#include <QProcess>

#include "controllerbase.h"

Q_DECLARE_LOGGING_CATEGORY(LG_INFO_CONTROLLER)

#define SYSTEM_TIME_BC_INTERVAL 1000
#define SYSTEM_TEMP_HC_INTERVAL 10000

class InfoController : public ControllerBase
{
    Q_OBJECT
public:
    explicit InfoController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return INFO_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QString getEnumName();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    static QString CONTROLLER_NAME;

    bool timeIsOffset();

    VALUE_BC_INTERVAL getValueBCInterval(int index=-1);

protected:
    void onInit();

    void onMqttConnected();

    void onValueChanged(int index, QVariant value);

    QTimer m_systemTimeTimer;

    QTimer m_systemTempTimer;

    bool m_timeIsOffset = false;

    QProcess m_readSystemTempProcess;

signals:


    void timeIsOffsetChanged(bool isOffset);

private slots:
    void onReadSystemTemp(int exitCode, QProcess::ExitStatus exitStatus);

public slots:
    void startReadSystemTemp();

};

#endif // INFOCONTROLLER_H
