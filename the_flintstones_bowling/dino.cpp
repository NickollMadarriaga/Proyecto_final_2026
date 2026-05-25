#include "dino.h"

#include <QPixmap>
#include <cmath>

Dino::Dino(Roca *rocaRef)
{
    roca = rocaRef;

    QPixmap imagen(":/imagenes/dino.png");

    setPixmap(imagen.scaled(100,100));

    velocidad = 2;

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
    if(distancia < 80)
    {
        roca->cambiarVelocidad(-5);
    }

    if(distancia < 200)
    {
        alerta = true;
    }
    else
    {
        alerta = false;
    }

    if(alerta)
    {
        setPos(x() - 5, y());
    }
    else
    {
        setPos(x() + velocidad, y());

        if(x() > 900)
        {
            velocidad *= -1;
        }

        if(x() < 600)
        {
            velocidad *= -1;
        }
    }
}