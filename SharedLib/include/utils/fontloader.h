#ifndef FONTLOADER_H
#define FONTLOADER_H

#include <QObject>

class FontLoader : public QObject
{
    Q_OBJECT
public:
    explicit FontLoader(QObject *parent = nullptr);

    static bool loadFonts();

signals:

public slots:
};

#endif // FONTLOADER_H
