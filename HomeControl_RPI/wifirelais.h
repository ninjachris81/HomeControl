#ifndef WIFIRELAIS_H
#define WIFIRELAIS_H

#include <QObject>

class WifiRelais : public QObject
{
    Q_OBJECT
public:
    explicit WifiRelais(QObject *parent = nullptr);

signals:

public slots:
};

#endif // WIFIRELAIS_H