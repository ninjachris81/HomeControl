#ifndef RPIGPIOMANAGER_H
#define RPIGPIOMANAGER_H

#include <QObject>

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(LG_RPI_GPIO_MANAGER)

class RPIGpioManager : public QObject
{
    Q_OBJECT
public:
    explicit RPIGpioManager(QObject *parent = nullptr);

    void configureAsInput(int channel);
    void configureAsOutput(int channel);

    bool read(int channel);
    void write(int channel, bool value);

signals:

public slots:
};

#endif // RPIGPIOMANAGER_H
