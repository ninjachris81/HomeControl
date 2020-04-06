#ifndef DATALOGGERCONTROLLER_H
#define DATALOGGERCONTROLLER_H

#include <QObject>
#include <QSqlDatabase>
#include <QList>

#include "controllerbase.h"

#include "utils/qsqldatabasewrapper.h"
#include <QLoggingCategory>
#include "controller/settingscontroller.h"

Q_DECLARE_LOGGING_CATEGORY(LG_DATA_LOG_CONTROLLER)

class DataLoggerController : public ControllerBase
{
    Q_OBJECT
public:
    explicit DataLoggerController(QObject *parent = nullptr);

    void registerValue(ControllerBase *controller, int valueIndex);

    QString getName();

    CONTROLLER_TYPE getType() {
        return DATA_LOG_CONTROLLER;
    }

    QStringList getTopicPath();

    QStringList getLabelList();

    QVariant::Type getDefaultValueType();

    qint64 getValueLifetime(int index = -1);

    QString getEnumName();

    static QString CONTROLLER_NAME;

    static QString DB_TABLE_DATA_LOG;

protected:
    QMultiMap<ControllerBase*, int> m_Values;
    ControllerManager *m_controllerManager;

    QTimer m_LogTimer;
    QTimer m_LimitTimer;

    QSqlDatabase m_db;

    quint16 m_entryLimit;

    void onInit();

    void addEntry(ControllerBase::CONTROLLER_TYPE controllerType, int index, QVariant v, bool isValid);

protected slots:
    void onLogData();

    void onCheckLimit();

signals:
    void dataLogDataChanged();

public slots:
};

#endif // DATALOGGERCONTROLLER_H
