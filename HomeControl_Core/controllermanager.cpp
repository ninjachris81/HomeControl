#include "controllermanager.h"
#include "heatingcontroller.h"

ControllerManager::ControllerManager(AppConfiguration *appConfig, QObject *parent) : QObject(parent), m_appConfig(appConfig)
{
    ControllerBase* c = new HeatingController(m_appConfig);
    m_controllers.insert(c->metaObject()->className(), c);
}

void ControllerManager::init() {
    for (ControllerBase* c : m_controllers) {
        qDebug() << "Init" << c->getName();
        c->init();
    }
}
