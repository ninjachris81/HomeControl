#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "databridge.h"

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    DataBridge dataBridge;

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("DataBridge", &dataBridge);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
