#include "datastatisticsanalyzerlogic.h"

#include "utils/databasemanager.h"
#include "controller/dataloggercontroller.h"

#include <algorithm>

Q_LOGGING_CATEGORY(LG_DATA_STATISTICS_ANALYZER_LOGIC, "DataStatisticsAnalyserLogic");

QString DataStatisticsAnalyzerLogic::DB_TABLE_DATA_ANALYTICS = QStringLiteral("data_analytics");

DataStatisticsAnalyzerLogic::DataStatisticsAnalyzerLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, DATA_ANALYZER_LOGIC_INTERVAL, parent)
{

}

void DataStatisticsAnalyzerLogic::startMaintenance() {
    onMaintenance();            // execute at startup
}

void DataStatisticsAnalyzerLogic::onMaintenance() {
    if (lastCheck==0 || QDateTime::currentMSecsSinceEpoch() - lastCheck > DATA_ANALYZER_EXECUTION_INTERVAL) {
        lastCheck = QDateTime::currentMSecsSinceEpoch();
        executeCheck();
    }
}

void DataStatisticsAnalyzerLogic::executeCheck() {
    qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << Q_FUNC_INFO;

    executePowerCheck();
    executePVCheck();
}

void DataStatisticsAnalyzerLogic::executePowerCheck() {
    executeStandardAnalysis(ControllerBase::CONTROLLER_TYPE::POWER_CONTROLLER, EnumsDeclarations::POWERS_MAIN, DATA_ANALYTICS_TYPE::POWER_MAIN_MIN_PER_DAY, DATA_ANALYTICS_TYPE::POWER_MAIN_MAX_PER_DAY, DATA_ANALYTICS_TYPE::POWER_MAIN_AVG_PER_DAY);
}

void DataStatisticsAnalyzerLogic::executePVCheck() {
    executeStandardAnalysis(ControllerBase::CONTROLLER_TYPE::CURRENT_CONTROLLER, EnumsDeclarations::CURRENTS_PV, DATA_ANALYTICS_TYPE::NONE, DATA_ANALYTICS_TYPE::PV_CURRENT_MAX_PER_DAY, DATA_ANALYTICS_TYPE::PV_CURRENT_AVG_PER_DAY);
}

bool DataStatisticsAnalyzerLogic::getTimeSpan(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, QDateTime &from, QDateTime &to) {
    qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << Q_FUNC_INFO << DatabaseManager::instance()->db();

    QSqlQuery query(DatabaseManager::instance()->db());
    query.prepare("SELECT MIN(ts) FROM " + DataLoggerController::DB_TABLE_DATA_LOG + " WHERE controller=:controller AND value_index=:value_index AND (analytics_state=:analytics_state OR analytics_state IS NULL)");
    query.bindValue(":controller", controller);
    query.bindValue(":value_index", valueIndex);
    query.bindValue(":analytics_state", NOT_ANALYZED);

    if (query.exec()) {
        if (query.first()) {
            QDateTime firstEntry = QDateTime::fromMSecsSinceEpoch(query.value(0).toLongLong());

            qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "First Entry" << firstEntry;
            if (firstEntry.daysTo(QDateTime::currentDateTime()) >=2) {
                from = QDateTime(firstEntry.date(), QTime(0, 0, 0));
                to = QDateTime(firstEntry.date(), QTime(23, 59, 59, 999));
                return true;
            } else {
                qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "Not enough data to process";
            }
        } else {
            qCWarning(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "No data";
        }
    } else {
        qCWarning(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "Error while executing SQL" << query.lastError();
    }
    return false;
}

void DataStatisticsAnalyzerLogic::executeStandardAnalysis(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, DATA_ANALYTICS_TYPE min, DATA_ANALYTICS_TYPE max, DATA_ANALYTICS_TYPE avg) {
    qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << Q_FUNC_INFO << DatabaseManager::instance()->db();

    QSqlQuery query(DatabaseManager::instance()->db());
    query.prepare("SELECT MAX(ts), MIN(ts) FROM " + DataLoggerController::DB_TABLE_DATA_LOG + " WHERE controller=:controller AND value_index=:value_index AND (analytics_state=:analytics_state OR analytics_state IS NULL) AND is_valid=:is_valid");
    query.bindValue(":controller", controller);
    query.bindValue(":value_index", valueIndex);
    query.bindValue(":analytics_state", NOT_ANALYZED);
    query.bindValue(":is_valid", true);

    QDateTime from;
    QDateTime to;

    if (getTimeSpan(controller, valueIndex, from, to)) {
        qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "Processing span" << from << "to" << to;

        if (min!=NONE) {
            double minValue = getMinValue(controller, valueIndex, from, to);
            insertDataEntry(from, min, minValue);
        }
        if (min!=NONE) {
            double maxValue = getMaxValue(controller, valueIndex, from, to);
            insertDataEntry(from, max, maxValue);
        }
        if (min!=NONE) {
            double avgValue = getAvgValue(controller, valueIndex, from, to);
            insertDataEntry(from, avg, avgValue);
        }

        markAsAnalysed(controller, valueIndex, from, to);
    } else {
        qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "No data to process";
    }
}

double DataStatisticsAnalyzerLogic::getMinValue(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, QDateTime from, QDateTime to) {
    qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << Q_FUNC_INFO;

    QSqlQuery query(DatabaseManager::instance()->db());
    query.prepare("SELECT MIN(val) FROM " + DataLoggerController::DB_TABLE_DATA_LOG + " WHERE controller=:controller AND value_index=:value_index AND ts>=:from AND ts<=:to AND is_valid=:is_valid");
    query.bindValue(":controller", controller);
    query.bindValue(":value_index", valueIndex);
    query.bindValue(":from", from.toMSecsSinceEpoch());
    query.bindValue(":to", to.toMSecsSinceEpoch());
    query.bindValue(":is_valid", true);

    QVariant val = getSingleValue(query);
    if (val.isValid()) {
        return val.toDouble();
    } else {
        return 0.0;
    }
}

double DataStatisticsAnalyzerLogic::getMaxValue(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, QDateTime from, QDateTime to) {
    qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << Q_FUNC_INFO;

    QSqlQuery query(DatabaseManager::instance()->db());
    query.prepare("SELECT MAX(val) FROM " + DataLoggerController::DB_TABLE_DATA_LOG + " WHERE controller=:controller AND value_index=:value_index AND ts>=:from AND ts<=:to AND is_valid=:is_valid");
    query.bindValue(":controller", controller);
    query.bindValue(":value_index", valueIndex);
    query.bindValue(":from", from.toMSecsSinceEpoch());
    query.bindValue(":to", to.toMSecsSinceEpoch());
    query.bindValue(":is_valid", true);

    QVariant val = getSingleValue(query);
    if (val.isValid()) {
        return val.toDouble();
    } else {
        return 0.0;
    }
}

double DataStatisticsAnalyzerLogic::getAvgValue(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, QDateTime from, QDateTime to, double invalidValue) {
    qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << Q_FUNC_INFO << from.toMSecsSinceEpoch() << to.toMSecsSinceEpoch();

    QSqlQuery query(DatabaseManager::instance()->db());
    query.prepare("SELECT ts, val, is_valid FROM " + DataLoggerController::DB_TABLE_DATA_LOG + " WHERE controller=:controller AND value_index=:value_index AND ts>=:from AND ts<=:to ORDER BY ts ASC");
    query.bindValue(":controller", controller);
    query.bindValue(":value_index", valueIndex);
    query.bindValue(":from", from.toMSecsSinceEpoch());
    query.bindValue(":to", to.toMSecsSinceEpoch());

    double averagesPerMinute[60];
    double averageMinutesPerHour[60];
    double averageHoursPerDay[24];

    std::fill(std::begin(averagesPerMinute), std::end(averagesPerMinute), 0.0);     // reset array
    std::fill(std::begin(averageMinutesPerHour), std::end(averageMinutesPerHour), 0.0);     // reset array
    std::fill(std::begin(averageHoursPerDay), std::end(averageHoursPerDay), 0.0);     // reset array

    qint64 totalSec = 0;
    qint64 currentSec = 0;
    quint16 currentMinute = 0;
    quint16 currentHour = 0;
    double currentVal = invalidValue;

    if (query.exec()) {
        quint16 entryCount = 0;
        while(query.next()) {
            qint64 ts = query.value(0).toLongLong();
            double val = query.value(1).toDouble();
            bool isValid = query.value(2).toBool();

            if (!isValid) {
                val = invalidValue;      // assume 0 if not valid
            }
            entryCount++;

            qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << ts << val;

            qint64 compTs = from.toMSecsSinceEpoch();

            while (compTs <= ts) {
                currentVal = val;
                currentSec++;
                totalSec++;
                compTs = (from.toMSecsSinceEpoch() + (totalSec * 1000));

                //qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << (compTs <= ts) << ":" << compTs << ":" << ts << " -- " << currentHour << ", " << currentMinute << ", " << currentSec << " -> " << val;

                if (currentSec==60) {
                    // shift
                    averageMinutesPerHour[currentMinute] = getAverage(averagesPerMinute, 60);
                    std::fill(std::begin(averagesPerMinute), std::end(averagesPerMinute), 0.0);     // reset array
                    currentMinute++;
                    currentSec = 0;
                }

                if (currentMinute==60) {
                    // shift
                    averageHoursPerDay[currentHour] = getAverage(averageMinutesPerHour, 60);
                    std::fill(std::begin(averagesPerMinute), std::end(averageMinutesPerHour), 0.0);     // reset array
                    currentHour++;
                    currentMinute = 0;
                }

                if (currentHour==24 && currentMinute==0 && currentSec==0) {
                    // should not happen
                    qCWarning(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "Finished, but more data";
                    query.last();
                    break;
                }

                averagesPerMinute[currentSec] = currentVal;
            }
        }

        qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "Processed" << entryCount << "entries";

        printValues(averageHoursPerDay, 24);

        return getAverage(averageHoursPerDay, 24);
    } else {
        qCWarning(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "Error while executing SQL" << query.lastError();
    }
    return 0.0;
}

double DataStatisticsAnalyzerLogic::getAverage(double* values, int count) {
    double val = 0.0;

    for (int i=0;i<count;i++) {
        val+=values[i];
    }
    return val/count;
}

void DataStatisticsAnalyzerLogic::printValues(double* values, int count) {
    for (int i=0;i<count;i++) {
        qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << i << " -> " << values[i];
    }
}

bool DataStatisticsAnalyzerLogic::checkExists(QDateTime ts, DATA_ANALYTICS_TYPE type) {
    qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << Q_FUNC_INFO;

    QSqlQuery query(DatabaseManager::instance()->db());
    query.prepare("SELECT COUNT(*) FROM " + DataStatisticsAnalyzerLogic::DB_TABLE_DATA_ANALYTICS + " WHERE ts=:ts AND type=:type");
    query.bindValue(":ts", ts.toMSecsSinceEpoch());
    query.bindValue(":type", type);

    QVariant val = getSingleValue(query);
    if (val.isValid()) {
        return val.toInt()>0;
    } else {
        return false;
    }
}

QVariant DataStatisticsAnalyzerLogic::getSingleValue(QSqlQuery &query) {
    if (query.exec()) {
        query.first();
        return query.value(0);
    } else {
        qCWarning(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "Error while executing SQL" << query.lastError();
    }
    return QVariant();
}

bool DataStatisticsAnalyzerLogic::insertDataEntry(QDateTime ts, DATA_ANALYTICS_TYPE type, double val) {
    qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << Q_FUNC_INFO;

    if (!checkExists(ts, type)) {
        QSqlQuery query(DatabaseManager::instance()->db());

        query.prepare("INSERT INTO " + DataStatisticsAnalyzerLogic::DB_TABLE_DATA_ANALYTICS + " (ts, type, val) VALUES (:ts, :type, :val)");
        query.bindValue(":ts", ts.toMSecsSinceEpoch());
        query.bindValue(":type", type);
        query.bindValue(":val", val);

        return query.exec();
    } else {
        qCWarning(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "Entry already exists" << ts << type;
    }

    return false;
}

void DataStatisticsAnalyzerLogic::markAsAnalysed(ControllerBase::CONTROLLER_TYPE controller, int valueIndex, QDateTime from, QDateTime to) {
    qCDebug(LG_DATA_STATISTICS_ANALYZER_LOGIC) << Q_FUNC_INFO;

    QSqlQuery query(DatabaseManager::instance()->db());
    query.prepare("UPDATE " + DataLoggerController::DB_TABLE_DATA_LOG + " SET analytics_state=:analytics_state WHERE controller=:controller AND value_index=:value_index AND ts>=:from AND ts<=:to");
    query.bindValue(":analytics_state", ANALYSED);
    query.bindValue(":controller", controller);
    query.bindValue(":value_index", valueIndex);
    query.bindValue(":from", from.toMSecsSinceEpoch());
    query.bindValue(":to", to.toMSecsSinceEpoch());

    if (!query.exec()) {
        qCWarning(LG_DATA_STATISTICS_ANALYZER_LOGIC) << "Error while update" << query.lastError();
    }
}
