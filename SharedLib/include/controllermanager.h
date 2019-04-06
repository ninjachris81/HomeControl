#ifndef CONTROLLERMANAGER_H
#define CONTROLLERMANAGER_H

#include <QObject>
#include <QMap>
#include "controllerbase.h"
#include "appconfiguration.h"

class ControllerManager : public QObject
{
    Q_OBJECT
public:
    explicit ControllerManager(QObject *parent = nullptr);

    void init(AppConfiguration* appConfig);

    void registerController(ControllerBase *controller);

private:
    QMap<QString, ControllerBase*> m_controllers;
    AppConfiguration* m_appConfig;

signals:

public slots:
};

#endif // CONTROLLERMANAGER_H
