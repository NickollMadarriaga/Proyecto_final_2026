#include "dino.h"

#include <QPixmap>
#include <cmath>

Dino::Dino(Roca *rocaRef)
{
    roca = rocaRef;

    QPixmap imagen(":/imagenes/dino.png");

    setPixmap(imagen.scaled(100,100));

    velocidad = 1.5;
    yaGolpeo = false;

    alerta = false;

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout,
            this, &Dino::mover);

    timer->start(16);
}

void Dino::mover()
{
    float dx = roca->x() - x();

    float dy = roca->y() - y();

    float distancia = sqrt(dx*dx + dy*dy);

    if(distancia < 150)
    {
        if(dx > 0)
        {
            setPos(x() + velocidad, y());
        }
        else
        {
            setPos(x() - velocidad, y());
        }

        if(distancia < 80 && !yaGolpeo)
        {
            yaGolpeo = true;

            roca->cambiarVelocidad(-3);
        }

    }
}