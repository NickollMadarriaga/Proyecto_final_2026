#include "personaje.h"
Personaje::Personaje(QObject *parent)
    : QObject(parent), QGraphicsPixmapItem(), velocidad(0) {}