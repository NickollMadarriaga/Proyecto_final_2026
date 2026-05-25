#include "roca.h"
#include "totems.h"

Roca::Roca()
{
    QPixmap rocaImagen(":/imagenes/roca.png");

    setPixmap(rocaImagen.scaled(60,60));

    velocidadX = 0;
    velocidadY = 0;

    gravedad = 0.4;

    usarGravedad = false;
    superRoca = false;

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
    if(usarGravedad)
    {
        velocidadY += gravedad;

        setPos(x() + velocidadX,
               y() + velocidadY);
    }
    else
    {
        setPos(x() + velocidadX, y());
    }
    QList<QGraphicsItem*> colisiones = collidingItems();

    for(QGraphicsItem *item : colisiones)
    {
        Totems *totems = dynamic_cast<Totems*>(item);

        if(totems && !totems->derribado)
        {
            totems->derribado = true;

            totems->setRotation(90);
        }
    }

    velocidadX *= 0.995;

    if(velocidadX < 0.1)
    {
        velocidadX = 0;
    }
    if(abs(velocidadX) < 0.5 && superRoca)
    {
        superRoca = false;

        QPixmap rocaImagen(":/imagenes/roca.png");

        setPixmap(rocaImagen.scaled(60,60));
    }
}
void Roca::cambiarVelocidad(float nuevaVelocidad)
{
    velocidadX = nuevaVelocidad;
}
void Roca::lanzarParabolico(float vx, float vy)
{
    velocidadX = vx;

    velocidadY = vy;

    usarGravedad = true;
}
void Roca::activarSuperRoca()
{
    superRoca = true;

    QPixmap rocaImagen(":/imagenes/roca.png");

    setPixmap(rocaImagen.scaled(100,100));
}