#include "nivelcueva.h"

#include "roca.h"
#include "totems.h"

#include <QPixmap>

NivelCueva::NivelCueva(QGraphicsScene *scene)
    : Nivel(scene)
{

}

void NivelCueva::cargarNivel()
{
    scene->clear();

    QPixmap fondo(":/images/fondo.png");

    scene->setBackgroundBrush(fondo);

    scene->addRect(0,500,1000,100);

    roca = new Roca();

    roca->setPos(50,460);

    scene->addItem(roca);

    Totems *totem1 = new Totems();
    Totems *totem2 = new Totems();
    Totems *totem3 = new Totems();

    totem1->setPos(700,420);
    totem2->setPos(750,420);
    totem3->setPos(725,340);

    scene->addItem(totem1);
    scene->addItem(totem2);
    scene->addItem(totem3);
}
