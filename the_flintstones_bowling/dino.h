#ifndef DINO_H
#define DINO_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include "roca.h"
#include "personaje.h"

class Dino : public Personaje
{
public:
    explicit Dino(Roca *rocaRef);

public slots:
    void mover();

private:
    Roca *roca;

    QTimer *timer;

    float velocidad;

    bool alerta;
    bool yaGolpeo;
};

#endif // DINO_H
