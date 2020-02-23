#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "utils/appconfiguration.h"
#include "utils/apptranslator.h"

#include "databridge.h"

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    Q_INIT_RESOURCE(sharedcomponents);

    QGuiApplication app(argc, argv);

    AppConfiguration appConfig;
    AppTranslator appTranslator(&appConfig);

    DataBridge dataBridge(&appConfig);

    QQmlApplicationEngine engine;

    engine.addImportPath(":/StyleDef/qml");
    engine.addImportPath(":/SharedComponents/qml");

    engine.rootContext()->setContextProperty("DataBridge", &dataBridge);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
