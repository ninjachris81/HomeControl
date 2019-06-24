#ifndef SOLARCONTROLLER_H
#define SOLARCONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QDateTime>

#define PIN_MOTOR_RELAY 8

class SolarController : public QObject
{
    Q_OBJECT
public:
    explicit SolarController(QObject *parent = nullptr);

    enum MotorDirection {
        DIRECTION_OFF,
        DIRECTION_UP,
        DIRECTION_DOWN,
    };

    enum MotorPosition {
        POSITION_INITIAL,
        POSITION_1,
        POSITION_2,
        POSITION_3,
        POSITION_FINAL
    };

    bool init();

protected:
    QTimer mMaintenanceTimer;

    QDateTime initStarted;

    void switchRelay(MotorDirection direction);

    MotorPosition m_currentPosition;

signals:

private slots:
    void onMaintenance();

public slots:
};

#endif // SOLARCONTROLLER_H
