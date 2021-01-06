#ifndef SERIALEXTENSIONCLIENT_H
#define SERIALEXTENSIONCLIENT_H

#include <QObject>
#include <QLoggingCategory>

#include "utils/serialportreader.h"

#include "controller/controllermanager.h"
#include "controller/eventcontroller.h"
#include "controller/currentcontroller.h"
#include "controller/powercontroller.h"

Q_DECLARE_LOGGING_CATEGORY(LG_SERIAL_EXTENSION_CLIENT)

#define SERIAL_TIMEOUT_MS 5000

class SerialExtensionClient : public QObject
{
    Q_OBJECT
public:
    explicit SerialExtensionClient(ControllerManager* controllerManager, AppConfiguration &appConfig, QObject *parent = nullptr);

signals:

private slots:
    void onLineReceived(QByteArray data);

private:
    EventController* m_eventController;
    CurrentController* m_currentController;
    PowerController* m_powerController;

    SerialPortReader serialPortReader;

};

#endif // SERIALEXTENSIONCLIENT_H
