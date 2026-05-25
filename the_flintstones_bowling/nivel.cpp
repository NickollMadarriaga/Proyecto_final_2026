#include "nivel.h"

Nivel::Nivel(QGraphicsScene *scene)
{
    this->scene = scene;
}
Roca* Nivel::getRoca()
{
    return roca;
}
