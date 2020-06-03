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
    Q_PROPERTY(DataLoggerSqlListModel* insideTempModel READ insideTempModel NOTIFY insideTempModelChanged)
    Q_PROPERTY(DataLoggerSqlListModel* solarTempModel READ solarTempModel NOTIFY solarTempModelChanged)

    Q_PROPERTY(DataLoggerSqlListModel* pvModel READ pvModel NOTIFY pvModelChanged)
    Q_PROPERTY(DataLoggerSqlListModel* hcPumpModel READ hcPumpModel NOTIFY hcPumpModelChanged)
    Q_PROPERTY(DataLoggerSqlListModel* heatingPumpModel READ heatingPumpModel NOTIFY heatingPumpModelChanged)

    DataLoggerSqlListModel* tankTempModel();
    DataLoggerSqlListModel* hcTempModel();
    DataLoggerSqlListModel* insideTempModel();
    DataLoggerSqlListModel* solarTempModel();

    DataLoggerSqlListModel* pvModel();
    DataLoggerSqlListModel* hcPumpModel();
    DataLoggerSqlListModel* heatingPumpModel();

protected:
    DataLoggerSqlListModel* m_tempTempModel;
    DataLoggerSqlListModel* m_hcTempModel;
    DataLoggerSqlListModel* m_insideTempModel;
    DataLoggerSqlListModel* m_solarTempModel;

    DataLoggerSqlListModel* m_pvModel;
    DataLoggerSqlListModel* m_hcPumpModel;
    DataLoggerSqlListModel* m_heatingPumpModel;

signals:
    void tankTempModelChanged();
    void hcTempModelChanged();
    void insideTempModelChanged();
    void solarTempModelChanged();
    void pvModelChanged();
    void hcPumpModelChanged();
    void heatingPumpModelChanged();

public slots:
};

Q_DECLARE_METATYPE( DataLoggerWrapper* );

#endif // DATALOGGERWRAPPER_H
