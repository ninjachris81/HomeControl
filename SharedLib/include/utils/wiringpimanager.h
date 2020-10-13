#ifndef WIRINGPIMANAGER_H
#define WIRINGPIMANAGER_H

#include <QObject>

class WiringPiManager : public QObject
{
    Q_OBJECT
public:
    explicit WiringPiManager(QObject *parent = nullptr);

    static void initWiringPi();

private:
    static bool isInitialized;

signals:

public slots:
};

#endif // WIRINGPIMANAGER_H
