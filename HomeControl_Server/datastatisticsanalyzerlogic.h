#ifndef DATASTATISTICSANALYZERLOGIC_H
#define DATASTATISTICSANALYZERLOGIC_H

#include <QObject>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlResult>
#include <QSqlError>

#include <QLoggingCategory>
#include "controller/logiccontroller.h"

#define DATA_ANALYZER_LOGIC_INTERVAL 60000
#define DATA_ANALYZER_EXECUTION_INTERVAL 3600000        // 1 hour

Q_DECLARE_LOGGING_CATEGORY(LG_DATA_STATISTICS_ANALYZER_LOGIC)

class DataStatisticsAnalyzerLogic : public LogicController
{
    Q_OBJECT
public:
    explicit DataStatisticsAnalyzerLogic(ControllerManager *controllerManager, QObject *parent = nullptr);

    enum DATA_ANALYTICS_TYPE {
        NONE = -1,
        POWER_MAIN_MIN_PER_DAY = 0,
        POWER_MAIN_MAX_PER_DAY,
        POWER_MAIN_AVG_PER_DAY,
        PV_CURRENT_MAX_PER_DAY,
        PV_CURRENT_AVG_PER_DAY
    };

    enum DATA_ANALYTICS_STATE {
        NOT_ANALYZED,
        ANALYSED
    };

    static QString DB_TABLE_DATA_ANALYTICS;

protected:
    qint64 lastCheck = 0;

    void startMaintenance();

    void executeCheck();

    void executePowerCheck();

    void executePVCheck();

    void executeStandardAnalysis(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, DATA_ANALYTICS_TYPE min, DATA_ANALYTICS_TYPE max, DATA_ANALYTICS_TYPE avg);

    bool getTimeSpan(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, QDateTime &from, QDateTime &to);

    double getMinValue(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, QDateTime from, QDateTime to);
    double getMaxValue(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, QDateTime from, QDateTime to);
    double getAvgValue(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, QDateTime from, QDateTime to, double invalidValue = 0.0);

    void markAsAnalysed(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, QDateTime from, QDateTime to);

    bool checkExists(QDateTime ts, DATA_ANALYTICS_TYPE type);

    QVariant getSingleValue(QSqlQuery &query);

    double getAverage(double* values, int count);

    void printValues(double* values, int count);

    bool insertDataEntry(QDateTime ts, DATA_ANALYTICS_TYPE type, double val);

signals:

public slots:
    void onMaintenance();

};

#endif // DATASTATISTICSANALYZERLOGIC_H
