#include "nivelbarranco.h"
#include <QPixmap>
#include <QPen>
#include <QBrush>
#include <QLinearGradient>

static const int PIX=30,  PIY=430, PIW=240;
static const int PMX=390, PMY=360, PMW=180;
static const int PDX=710, PDY=280, PDW=220;
static const int PH=20;
static const int SUELO2=530, PHH2=151, RH2=55, TH2=80, DH2=96;

NivelBarranco::NivelBarranco(QGraphicsScene *s) : Nivel(s) {
    fred=nullptr; dino=nullptr;
    rocaIniX = PIX + 160;
    rocaIniY = PIY - RH2;
}

void NivelBarranco::cargarNivel() {
    scene->clear(); listaTotems.clear(); plats.clear();

    QPixmap fondo(":/imagenes/fondo2.png");
    if (!fondo.isNull())
        scene->setBackgroundBrush(fondo.scaled(1000,600,
                                               Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    else {
        QLinearGradient grad(0,0,0,600);
        grad.setColorAt(0,   QColor(80,120,160));
        grad.setColorAt(0.6, QColor(140,180,100));
        grad.setColorAt(1,   QColor(100,75,40));
        scene->setBackgroundBrush(grad);
    }

    scene->addRect(0,SUELO2,1000,70,
                   QPen(Qt::NoPen),QBrush(QColor(90,65,30,200)))->setZValue(0);

    mkPlataformas();

    roca = new Roca();
    roca->sueloY = SUELO2 - RH2;
    roca->setPos(rocaIniX, rocaIniY);
    roca->setZValue(5);
    scene->addItem(roca);

    fred = new PedroPicapiedra();
    fred->rocaRef = roca;
    fred->setPos(PIX + 5, PIY - PHH2);
    fred->setZValue(6);
    scene->addItem(fred);

    mkTotems();

    dino = new Dino(roca);
    // En cargarNivel(), cambia la línea del dino:
    dino->setPos(PDX + PDW + 10, PDY - DH2);   // patrulla a la derecha de los tótems
    dino->setZValue(4);
    scene->addItem(dino);

    configurarFisica();
}

void NivelBarranco::mkPlataformas() {
    auto mkPlat = [&](int x, int y, int w) {
        scene->addRect(x, y, w, PH,
                       QPen(QColor(60,38,10),2),
                       QBrush(QColor(130,88,38)))->setZValue(2);
        scene->addRect(x, y, w, 4,
                       QPen(Qt::NoPen),
                       QBrush(QColor(180,130,70)))->setZValue(3);
        scene->addRect(x+w/2-18, y+PH, 36, SUELO2-y-PH,
                       QPen(QColor(50,32,8),1),
                       QBrush(QColor(105,72,30)))->setZValue(1);
        plats.append(QRectF(x,y,w,PH));
    };
    mkPlat(PIX, PIY, PIW);
    mkPlat(PMX, PMY, PMW);
    mkPlat(PDX, PDY, PDW);
}

void NivelBarranco::mkTotems() {

    const int SEP_X  = 30;        // separación horizontal entre tótems
    const int SEP_Y  = TH2 ;  // separación vertical
    const int BASE_X = PDX + 30;  // x del primer tótem base
    const int BASE_Y = PDY - TH2+15; // y base: apoyados en la plataforma

    QVector<QPointF> pos = {
        { BASE_X,             BASE_Y         },
        { BASE_X + SEP_X,     BASE_Y         },
        { BASE_X + SEP_X*2,   BASE_Y         },
        { BASE_X + SEP_X*0.5f, BASE_Y - SEP_Y },
        { BASE_X + SEP_X*1.5f, BASE_Y - SEP_Y },
        { BASE_X + SEP_X,      BASE_Y - SEP_Y*2 },
        };

    totemsRestantes = pos.size();
    for (auto &p : pos) {
        Totems *t = new Totems();
        t->setPos(p); t->setZValue(3);
        listaTotems.append(t); scene->addItem(t);
    }
}

void NivelBarranco::configurarFisica() {
    if (roca) roca->usarGravedad = true;
}

void NivelBarranco::actualizarFisica() {
    if (!roca || !roca->estaActiva) return;
    for (const QRectF &pl : plats) {
        QRectF rb = roca->sceneBoundingRect();
        if (rb.intersects(pl) && roca->velocidadY > 0) {
            roca->setY(pl.top() - RH2);
            roca->velocidadY = 0;
            roca->usarGravedad = false;
        }
    }
}

void NivelBarranco::avance(int fase) {
    if (fase==1 && fred) fred->avance(1);
}