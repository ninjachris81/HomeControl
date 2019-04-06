#include "databridge.h"
#include <QDebug>

DataBridge::DataBridge(AppConfiguration *appConfig, QObject *parent) : QObject(parent), m_appConfig(appConfig)
{

    m_controllerManager.registerController(&m_tempController);
    m_controllerManager.registerController(&m_errorController);
    m_controllerManager.init(appConfig);

    m_tempListModelController = new ControllerListModel(&m_tempController);
    m_errorListModelController = new ErrorControllerListModel(&m_errorController);
}


ControllerListModel* DataBridge::tempListModelController() {
    return m_tempListModelController;
}

ErrorControllerListModel *DataBridge::errorListModelController() {
    return m_errorListModelController;
}
