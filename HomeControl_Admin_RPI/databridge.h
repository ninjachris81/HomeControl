#ifndef DATABRIDGE_H
#define DATABRIDGE_H

#include <QObject>
#include "utils/databridgebase.h"

class DataBridge : public DataBridgeBase
{
    Q_OBJECT
public:
    explicit DataBridge(AppConfiguration *appConfig, QObject *parent = nullptr);

public slots:
};

#endif // DATABRIDGE_H
