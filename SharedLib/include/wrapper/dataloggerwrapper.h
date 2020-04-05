#ifndef DATALOGGERWRAPPER_H
#define DATALOGGERWRAPPER_H

#include <QObject>
#include <QQmlEngine>
#include <QtGlobal>

#include "controllerwrapper.h"
#include "controller/dataloggercontroller.h"

#include "sqllistmodel/dataloggersqllistmodel.h"

class DataLoggerWrapper : public ControllerWrapper
{
    Q_OBJECT
public:
    explicit DataLoggerWrapper(DataLoggerController *controller);
    ~DataLoggerWrapper();

    Q_PROPERTY(DataLoggerSqlListModel* tankTempModel READ tankTempModel NOTIFY tankTempModelChanged)

    Q_PROPERTY(DataLoggerSqlListModel* hcTempModel READ hcTempModel NOTIFY hcTempModelChanged)

    DataLoggerSqlListModel* tankTempModel();
    DataLoggerSqlListModel* hcTempModel();

protected:
    DataLoggerSqlListModel* m_tempTempModel;
    DataLoggerSqlListModel* m_hcTempModel;

signals:
    void tankTempModelChanged();
    void hcTempModelChanged();

public slots:
};

Q_DECLARE_METATYPE( DataLoggerWrapper* );

#endif // DATALOGGERWRAPPER_H
