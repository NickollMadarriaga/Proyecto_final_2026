#include "nivelbarranco.h"
#include <QPixmap>
#include <QPen>
#include <QBrush>

static const int SY2=500, PHH2=151, RH2=55, TH2=104;

NivelBarranco::NivelBarranco(QGraphicsScene *s) : Nivel(s) {
    fred=nullptr; dino=nullptr;
    rocaIniX=140; rocaIniY=SY2-RH2;
}

void NivelBarranco::cargarNivel() {
    scene->clear(); listaTotems.clear(); plats.clear();

    QPixmap fondo(":/imagenes/fondo2.png");
    if(!fondo.isNull())
        scene->setBackgroundBrush(fondo.scaled(1000,600,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    else
        scene->setBackgroundBrush(QBrush(QColor(55,85,125)));

    scene->addRect(0,SY2,1000,100,QPen(Qt::NoPen),QBrush(QColor(80,58,28,190)))->setZValue(0);

    mkPlataformas();

    roca=new Roca();
    roca->sueloY=SY2-RH2;
    roca->setPos(rocaIniX,rocaIniY);
    roca->setZValue(5);
    scene->addItem(roca);

    fred=new PedroPicapiedra();
    fred->rocaRef=roca;
    fred->setPos(5,SY2-PHH2);
    fred->setZValue(4);
    scene->addItem(fred);

    mkTotems();

    dino=new Dino(roca);
    dino->setPos(500,SY2-100);
    dino->setZValue(4);
    scene->addItem(dino);

    configurarFisica();
}

void NivelBarranco::mkPlataformas() {
    struct PD{int x,y,w;};
    QVector<PD> defs={{280,400,165},{500,310,155},{710,225,160},{420,255,125}};
    for(auto&d:defs){
        scene->addRect(d.x,d.y,d.w,18,
                       QPen(QColor(55,35,8),2),
                       QBrush(QColor(108,72,32)))->setZValue(2);
        plats.append(QRectF(d.x,d.y,d.w,18));
    }
}

void NivelBarranco::mkTotems() {
    QVector<QPointF> pos={
                            {310, 400-TH2},
                            {530, 310-TH2},
                            {740, 225-TH2},
                            {450, 255-TH2},
                            {380, SY2-TH2},
                            };
    totemsRestantes=pos.size();
    for(auto&p:pos){
        Totems*t=new Totems();
        t->setPos(p); t->setZValue(3);
        listaTotems.append(t); scene->addItem(t);
    }
}

void NivelBarranco::configurarFisica() {
    if(roca) roca->usarGravedad=true;
}
void NivelBarranco::actualizarFisica() {
    if(!roca||!roca->estaActiva) return;
    for(const QRectF&pl:plats){
        QRectF rb=roca->sceneBoundingRect();
        if(rb.intersects(pl)&&roca->velocidadY>0){
            roca->setY(pl.top()-RH2);
            roca->velocidadY=0;
        }
    }
}
void NivelBarranco::avance(int fase){
    if(fase==1&&fred) fred->avance(1);
}