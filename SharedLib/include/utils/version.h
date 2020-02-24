#ifndef VERSION_H
#define VERSION_H

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_BUILD 0

#include <QString>

namespace Version {
    static QString getVersion() {
        return QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_BUILD);
    }
}

#endif // VERSION_H
