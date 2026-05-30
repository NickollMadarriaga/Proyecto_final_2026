#include "nivel.h"

Nivel::Nivel(QGraphicsScene *scene)
{
    this->scene = scene;
    roca = nullptr;
    totemsRestantes = 0;
    tirosRestantes  = 5;
}

Nivel::~Nivel() {}

Roca* Nivel::getRoca()
{
    return roca;
}

int Nivel::getTotemsRestantes()
{
    return totemsRestantes;
}

bool Nivel::nivelCompletado()
{
    for (Totems *totems : listaTotems) {
        if (!totems->estaDerrribado())
            return false;
    }
    return true;
}

void Nivel::resetearRoca()
{
    if (roca) roca->resetear(50, scene->height() - 140);
}