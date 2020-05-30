#ifndef PVCONTROLLER_H
#define PVCONTROLLER_H

#include <QObject>
#include "controllerbase.h"
#include <QLoggingCategory>
#include <QTimer>

Q_DECLARE_LOGGING_CATEGORY(LG_PV_CONTROLLER)

class PvController : public ControllerBase
{
    Q_OBJECT
public:
    explicit PvController(QObject *parent = nullptr);

    QString getName();

    CONTROLLER_TYPE getType() {
        return EXTERNAL_PV_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QString getEnumName();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    qint64 getValueTrendLifetime(int index = -1);

    static QString CONTROLLER_NAME;

    //QVariant value(int index);

private:
    QTimer mCheckTimer;

protected slots:
    void onCheckDatabase();

protected:
    void onInit();

signals:

public slots:

};

#endif // PVCONTROLLER_H
