#include "httpserver.h"

#include "constants.h"
#include <QSqlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "utils/databasemanager.h"
#include "doorbelllogic.h"

Q_LOGGING_CATEGORY(LG_HTTP_SERVER, "Http Server")

HttpServer::HttpServer(ControllerManager *controllerManager, AppConfiguration &appConfig, QObject *parent) : HttpRequestHandler(parent)
{

    listenerSettings = new QSettings(QSettings::IniFormat, QSettings::Scope::UserScope, "com.ninjachris81.homecontrol", "HomeControl");
    listenerSettings->beginGroup("listener");
    listenerSettings->setValue("host", "");
    listenerSettings->setValue("port", 9090);
    listenerSettings->setValue("minThreads", 4);
    listenerSettings->setValue("maxThreads", 20);
    listenerSettings->setValue("cleanupInterval", 60000);
    listenerSettings->setValue("readTimeout", 30000);
    listenerSettings->setValue("maxRequestSize", 16000);
    listenerSettings->setValue("maxMultiPartSize", 10000000);

    httpServer = new HttpListener(listenerSettings, this, parent);
}

HttpServer::~HttpServer() {
    delete listenerSettings;
    delete httpServer;
}

void HttpServer::service(HttpRequest& request, HttpResponse& response) {
    qCDebug(LG_HTTP_SERVER) << Q_FUNC_INFO;

    QString path=request.getPath();

    if (path.startsWith(QString(MQTT_PATH_EVENTS).prepend("/").append("/last"))) {
        return writeContentLastEvents(request, response);
    }
}

void HttpServer::writeContentLastEvents(HttpRequest& request, HttpResponse& response) {
    qCDebug(LG_HTTP_SERVER) << Q_FUNC_INFO;


    QString lastTsStr = request.getParameter("lastTs");
    QString limitStr = request.getParameter("limit");

    if (!lastTsStr.isEmpty()) {
        qlonglong lastTs = lastTsStr.toLong();

        qCDebug(LG_HTTP_SERVER) << "lastTs" << lastTs << "limit" << limitStr;

        int limit = 1000;
        if (!limitStr.isEmpty()) {
            limit = limitStr.toInt();
        }


        QJsonDocument doc;
        QJsonObject obj;
        QJsonArray arr;

        QSqlQuery query(DatabaseManager::instance()->db());

        query.prepare("SELECT ts FROM " + DoorBellLogic::DOOR_BELL_EVENTS_DB_NAME + " WHERE ts > :ts ORDER BY ts DESC LIMIT :limit");
        query.bindValue(":ts", lastTs);
        query.bindValue(":limit", limit);
        query.exec();

        while (query.next()) {
            arr.append(QJsonValue::fromVariant(query.value(0)));
        }

        obj.insert("tsList", arr);
        doc.setObject(obj);
        writeSuccessResponse(response, doc.toJson());
    } else {
        writeErrorResponse(response);
    }
}

void HttpServer::writeSuccessResponse(HttpResponse& response, QByteArray data) {
    response.setStatus(200);
    response.setHeader("Content-Type", "text/json");
    response.write(data);
}

void HttpServer::writeErrorResponse(HttpResponse& response) {
    response.setStatus(400);
    response.write("");
}
