#include "include/controllermanager.h"
#include <QDebug>

ControllerManager::ControllerManager(QObject *parent) : QObject(parent)
{
}

void ControllerManager::init(AppConfiguration *appConfig) {
    m_appConfig = appConfig;

    for (ControllerBase* c : m_controllers) {
        qDebug() << "Init" << c->getName();
        c->init(appConfig);
    }
}

void ControllerManager::registerController(ControllerBase *controller) {
    m_controllers.insert(controller->getName(), controller);
}
