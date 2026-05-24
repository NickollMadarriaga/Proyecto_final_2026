#include "roca.h"
#include "totems.h"

Roca::Roca()
{
    setRect(0,0,40,40);

    velocidadX = 0;

    timer = new QTimer(this);

    connect(timer, &QTimer::timeout,
            this, &Roca::mover);

    timer->start(16);
}

void Roca::lanzar(float velocidad)
{
    velocidadX = velocidad;
}

void Roca::mover()
{
    setPos(x() + velocidadX, y());
    QList<QGraphicsItem*> colisiones = collidingItems();

    for(QGraphicsItem *item : colisiones)
    {
        Totems *totems = dynamic_cast<Totems*>(item);

        if(totems)
        {
            totems->setRotation(90);
        }
    }

    velocidadX *= 0.995;

    if(velocidadX < 0.1)
    {
        velocidadX = 0;
    }
}
