#include "include/utils/fontloader.h"
#include <QFontDatabase>
#include <QDebug>

FontLoader::FontLoader(QObject *parent) : QObject(parent)
{

}


bool FontLoader::loadFonts() {
    qDebug() << Q_FUNC_INFO;

    Q_ASSERT(QFontDatabase::addApplicationFont(":/fonts/DejaVuSans.ttf")>-1);
    Q_ASSERT(QFontDatabase::addApplicationFont(":/fonts/DejaVuSans-Bold.ttf")>-1);
    Q_ASSERT(QFontDatabase::addApplicationFont(":/fonts/DejaVuSans-Oblique.ttf")>-1);
    Q_ASSERT(QFontDatabase::addApplicationFont(":/fonts/DejaVuSans-BoldOblique.ttf")>-1);

    return true;
}
