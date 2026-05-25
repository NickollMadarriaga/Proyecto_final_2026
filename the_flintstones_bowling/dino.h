#ifndef DINO_H
#define DINO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

#include "roca.h"

class Dino : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit Dino(Roca *rocaRef);

public slots:
    void mover();

private:
    Roca *roca;

    QTimer *timer;

    float velocidad;

    bool alerta;
};

#endif // DINO_H
