#include "databridge.h"
#include "constants.h"
#include <QDebug>

DataBridge::DataBridge(AppConfiguration *appConfig, QObject *parent) : DataBridgeBase(appConfig, DEV_ID_ADMIN_RPI, parent)
{
}
