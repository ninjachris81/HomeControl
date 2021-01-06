#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include "httpserver/httplistener.h"
#include <QLoggingCategory>
#include <QSettings>

#include "controller/controllermanager.h"
#include "controller/eventcontroller.h"

Q_DECLARE_LOGGING_CATEGORY(LG_HTTP_SERVER)

using namespace stefanfrings;

class HttpServer : public HttpRequestHandler
{
    Q_OBJECT
public:
    explicit HttpServer(ControllerManager *controllerManager, AppConfiguration &appConfig, QObject *parent = nullptr);
    ~HttpServer();

    void service(HttpRequest& request, HttpResponse& response);

private:
    QSettings *listenerSettings;
    HttpListener *httpServer;

    void writeContentLastEvents(HttpRequest& request, HttpResponse& response);

    void writeSuccessResponse(HttpResponse& response, QByteArray data);

    void writeErrorResponse(HttpResponse& response);


signals:

public slots:
};

#endif // HTTPSERVER_H
