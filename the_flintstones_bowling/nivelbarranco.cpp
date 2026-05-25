#include "nivelbarranco.h"

#include "roca.h"
#include "totems.h"
#include "dino.h"

#include <QPixmap>

NivelBarranco::NivelBarranco(QGraphicsScene *scene)
    : Nivel(scene)
{

}

void NivelBarranco::cargarNivel()
{
    scene->clear();

    QPixmap fondo(":/images/fondo2.png");

    scene->setBackgroundBrush(fondo);

    scene->addRect(0,550,1000,50);

    scene->addRect(600,350,200,20);

    scene->addRect(800,250,150,20);

    roca = new Roca();

    roca->setPos(50,460);

    scene->addItem(roca);

    Totems *totem1 = new Totems();
    Totems *totem2 = new Totems();
    Totems *totem3 = new Totems();

    totem1->setPos(650,270);
    totem2->setPos(850,170);
    totem3->setPos(750,420);

    scene->addItem(totem1);
    scene->addItem(totem2);
    scene->addItem(totem3);

    Dino *dino = new Dino(roca);

    dino->setPos(600,400);

    scene->addItem(dino);
}
