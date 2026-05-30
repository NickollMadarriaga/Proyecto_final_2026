#include "totems.h"
#include <QPixmap>

Totems::Totems(QObject *parent) : QObject(parent), QGraphicsPixmapItem()
{
    QPixmap imagen(":/imagenes/Totem.png");
    if (imagen.isNull()) {
        // Fallback: dibujar un totem simple si no hay imagen
        QPixmap fallback(60, 120);
        fallback.fill(Qt::gray);
        setPixmap(fallback);
    } else {
        setPixmap(imagen.scaled(60, 120, Qt::KeepAspectRatio));
    }

    derribado = false;
    anguloRotacion = 0;
    velocidadRotacion = 3.0f;
    targetRotacion = 90.0f;

    // Pivote en la base del totem para animación realista
    setTransformOriginPoint(30, 120);

    timerAnimacion = new QTimer(this);
    connect(timerAnimacion, &QTimer::timeout, this, &Totems::actualizarAnimacion);
}

void Totems::derribar()
{
    if (!derribado) {
        derribado = true;
        animarCaida();
    }
}

bool Totems::estaDerrribado()
{
    return derribado;
}

void Totems::animarCaida()
{
    timerAnimacion->start(16);
}

void Totems::actualizarAnimacion()
{
    if (anguloRotacion < targetRotacion) {
        anguloRotacion += velocidadRotacion;
        if (anguloRotacion > targetRotacion) anguloRotacion = targetRotacion;
        setRotation(anguloRotacion);
    } else {
        timerAnimacion->stop();
    }
}