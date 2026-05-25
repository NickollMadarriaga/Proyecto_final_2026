#include "nivel.h"

Nivel::Nivel(QGraphicsScene *scene)
{
    this->scene = scene;
}
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
    for(Totems *totem : listaTotems)
    {
        if(!totem->derribado)
        {
            return false;
        }
    }

    return true;
}