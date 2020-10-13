#ifndef RPIANALOGREADER_H
#define RPIANALOGREADER_H

#include <QObject>

#define	AD_BASE 120

class RPIAnalogReader : public QObject
{
    Q_OBJECT
public:
    explicit RPIAnalogReader(QObject *parent = nullptr);

    void configure(int channel, int gain);

    int readValue(int channel);

private:
    int m_gains[4] = {0, 0, 0, 0};

signals:

public slots:
};

#endif // RPIANALOGREADER_H
