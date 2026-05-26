#include "nivelcueva.h"

#include "roca.h"
#include "totems.h"
#include "pedropicapiedra.h"
#include <QPixmap>

NivelCueva::NivelCueva(QGraphicsScene *scene)
    : Nivel(scene)
{

}

void NivelCueva::cargarNivel()
{
    scene->clear();

    QPixmap fondo(":/imagenes/fondo.png");

    scene->setBackgroundBrush(fondo);

    scene->addRect(0,500,1000,100);

    roca = new Roca();

    roca->setPos(50,460);

    scene->addItem(roca);
    totemsRestantes = 3;

    Totems *totem1 = new Totems();
    Totems *totem2 = new Totems();
    Totems *totem3 = new Totems();

    listaTotems.push_back(totem1);
    listaTotems.push_back(totem2);
    listaTotems.push_back(totem3);

    totem1->setPos(700,420);
    totem2->setPos(750,420);
    totem3->setPos(725,340);

    scene->addItem(totem1);
    scene->addItem(totem2);
    scene->addItem(totem3);
}
