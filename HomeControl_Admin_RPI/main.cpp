#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "utils/appconfiguration.h"
#include "utils/apptranslator.h"
#include "utils/fontloader.h"
#include "utils/version.h"

#include "databridge.h"

int main(int argc, char *argv[])
{
    qDebug() << "Version" << Version::getVersion();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    Q_INIT_RESOURCE(sharedcomponents);
    Q_INIT_RESOURCE(sharedfonts);
    Q_INIT_RESOURCE(sharedimages);

    QGuiApplication app(argc, argv);

    FontLoader::loadFonts();

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
