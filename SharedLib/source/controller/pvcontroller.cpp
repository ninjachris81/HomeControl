#include "include/controller/pvcontroller.h"
#include "include/controller/settingscontroller.h"
#include "include/controller/controllermanager.h"

#include "include/constants_qt.h"
#include "include/utils/databasemanager.h"
#include "include/controller/dataloggercontroller.h"

#include <QSqlQuery>
#include <QSqlError>

QString PvController::CONTROLLER_NAME = QStringLiteral("PvController");
Q_LOGGING_CATEGORY(LG_PV_CONTROLLER, "PvController");

PvController::PvController(QObject *parent) : ControllerBase(ControllerBase::VALUE_OWNER_SERVER, parent)
{
    connect(&mCheckTimer, &QTimer::timeout, this, &PvController::onCheckDatabase);
    mCheckTimer.setInterval(10000);
}

QString PvController::getName() {
    return CONTROLLER_NAME;
}

QStringList PvController::getTopicPath() {
    return QStringList() << MQTT_PATH_PVS;
}

QStringList PvController::getLabelList() {
    CONVERT_LABEL_LIST(EnumsDeclarations::PVS_LABELS);
}

QString PvController::getEnumName() {
    return "MQTT_PVS";
}

QVariant::Type PvController::getDefaultValueType() {
    return QVariant::Int;
}

qint64 PvController::getValueLifetime(int index) {
    Q_UNUSED(index)
    return LIFETIME_SHORT;
}

qint64 PvController::getValueTrendLifetime(int index) {
    Q_UNUSED(index)
    return VALUE_TT_FAST;
}

void PvController::onInit() {
    qCDebug(LG_PV_CONTROLLER) << Q_FUNC_INFO;

    if (m_parent->isServer()) {
        qCDebug(LG_PV_CONTROLLER) << "Starting check timer";
        mCheckTimer.start();
    }
}

void PvController::onCheckDatabase() {
    qCDebug(LG_PV_CONTROLLER) << Q_FUNC_INFO;

    QSqlDatabase db = DatabaseManager::instance()->db();
    QSqlQuery query(db);

    query.prepare("SELECT val FROM " + DataLoggerController::DB_TABLE_DATA_LOG + " WHERE controller=:controller and value_index=:mamps_value_index ORDER BY ts DESC LIMIT 1");
    query.bindValue(":controller", EXTERNAL_PV_CONTROLLER);
    query.bindValue(":mamps_value_index", MQTT_PATH_PVS_MAMPS);

    if (query.exec()) {
        if (query.first()) {
            setValue(EnumsDeclarations::PVS_MAMPS, query.value(0));
            publishValue(EnumsDeclarations::PVS_MAMPS);
        } else {
            qCWarning(LG_PV_CONTROLLER) << "no data available";
        }
    } else {
        qCWarning(LG_PV_CONTROLLER) << "Unable to execute statement" << query.lastError();
    }
}
