#include "personaje.h"

Personaje::Personaje(QObject *parent)
    : QObject(parent), QGraphicsPixmapItem()
{
    posX = 0;
    posY = 0;
    velocidad = 0;
}