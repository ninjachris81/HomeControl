#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "appconfiguration.h"
#include "databridge.h"
#include <QTimer>
#include "screensaverfilter.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    AppConfiguration appConfig;

    ScreenSaverFilter mef(&appConfig);
    app.installEventFilter(&mef);

    DataBridge dataBridge(&appConfig);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("DataBridge", &dataBridge);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
