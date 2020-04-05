#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "utils/appconfiguration.h"
#include "databridge.h"
#include "screensaverfilter.h"
#include "utils/apptranslator.h"
#include "utils/fontloader.h"

#include "constant_strings.h"
#include "sharedlib.h"

#include <QTimer>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    INIT_RESOURCES();

    QApplication app(argc, argv);

    FontLoader::loadFonts();

    AppConfiguration appConfig;
    AppTranslator appTranslator(&appConfig);

    ScreenSaverFilter mef(&appConfig);
    mef.init();
    app.installEventFilter(&mef);

    DataBridge dataBridge(&appConfig);

    QQmlApplicationEngine engine;

    engine.addImportPath(":/StyleDef/qml");
    engine.addImportPath(":/SharedComponents/qml");
    engine.addImportPath(":/SharedUtils/qml");

    engine.rootContext()->setContextProperty("DataBridge", &dataBridge);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
