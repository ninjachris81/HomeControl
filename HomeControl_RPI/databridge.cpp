#include "databridge.h"
#include <QDebug>

DataBridge::DataBridge(AppConfiguration *appConfig, QObject *parent) : DataBridgeBase(appConfig, DEV_ID_RPI1, parent)
{
}
