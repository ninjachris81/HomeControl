#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "utils/appconfiguration.h"
#include "databridge.h"
#include "screensaverfilter.h"
#include "utils/apptranslator.h"
#include "utils/fontloader.h"

#include "constant_strings.h"

#include <QTimer>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    Q_INIT_RESOURCE(sharedcomponents);
    Q_INIT_RESOURCE(sharedfonts);
    Q_INIT_RESOURCE(sharedimages);

    QGuiApplication app(argc, argv);

    FontLoader::loadFonts();

    AppConfiguration appConfig;
    AppTranslator appTranslator(&appConfig);

    ScreenSaverFilter mef(&appConfig);
    app.installEventFilter(&mef);

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
