#include "pvlogic.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#include "utils/databasemanager.h"
#include "controller/dataloggercontroller.h"
#include "controller/pvcontroller.h"

#include "constants_qt.h"

Q_LOGGING_CATEGORY(LG_PV_LOGIC, "PvLogic");

PvLogic::PvLogic(ControllerManager *controllerManager, QObject *parent) : LogicController(controllerManager, PV_LOGIC_INTERVAL, parent)
{

}

void PvLogic::startMaintenance() {

}

void PvLogic::onMaintenance() {
    QSqlQuery query(DatabaseManager::instance()->db());

    qulonglong lastTs = m_controllerManager->appConfig()->getValue("LAST_CHECKED_PV_TS", 0).toULongLong();

    query.prepare("SELECT ts, val FROM " + DataLoggerController::DB_TABLE_DATA_LOG + " WHERE ts>=:ts AND controller=:controller AND value_index=:value_index");
    query.bindValue(":ts", lastTs);
    query.bindValue(":controller", static_cast<PvController*>(m_controllerManager->getController(PvController::CONTROLLER_NAME))->getType());
    query.bindValue(":value_index", EnumsDeclarations::PVS_MAMPS);

    if (query.exec()) {

        if (query.size()>1) {        // need at least 2

            qulonglong currentTs = 0;

            while (query.next()) {

                if (currentTs>0) {

                }

                currentTs = query.value(0).toULongLong();
            }
        }
    } else {
        qCWarning(LG_PV_LOGIC) << "Failed to execute sql" << query.lastError();
    }


}
