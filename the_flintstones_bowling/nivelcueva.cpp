#include "nivelcueva.h"
#include <QPixmap>
#include <QPen>
#include <QBrush>

static const int SY  = 530;
static const int RH  = 55;
static const int PHH = 151;
static const int TH  = 120;
static const int TW  = 60;

NivelCueva::NivelCueva(QGraphicsScene *scene) : Nivel(scene) {
    pedro    = nullptr;
    rocaIniX = 160;
    rocaIniY = SY - RH;
}

void NivelCueva::cargarNivel() {
    scene->clear(); listaTotems.clear();

    QPixmap fondo(":/imagenes/fondo.png");
    if (!fondo.isNull())
        scene->setBackgroundBrush(fondo.scaled(1000,600,
                                               Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    else
        scene->setBackgroundBrush(QBrush(QColor(185,160,115)));

    scene->addRect(0, SY, 1000, 70,
                   QPen(Qt::NoPen), QBrush(QColor(110,75,35,150)))->setZValue(0);
    for (int i=0;i<10;i++)
        scene->addLine(50+i*95, SY+10, 100+i*95, SY+10,
                       QPen(QColor(85,55,20,140),2,Qt::DashLine))->setZValue(1);

    roca = new Roca();
    roca->sueloY = SY - RH;
    roca->setPos(rocaIniX, rocaIniY);
    roca->setZValue(5);
    scene->addItem(roca);

    pedro = new PedroPicapiedra();
    pedro->rocaRef = roca;
    pedro->setPos(0, SY - PHH);
    pedro->setZValue(6);
    scene->addItem(pedro);


    float baseY = (SY - TH)+30;
    float cx    = 750;
    float sepX  = 50;
    float sepY  = 35;

    struct P { float x, y; };
    QVector<P> pos = {
        {cx,            baseY},

        {cx + sepX,     baseY - sepY/2},
        {cx + sepX,     baseY + sepY/2},

        {cx + sepX*2,   baseY - sepY},
        {cx + sepX*2,   baseY},
        {cx + sepX*2,   baseY + sepY},

        {cx + sepX*3,   baseY - sepY*1.5},
        {cx + sepX*3,   baseY - sepY/2},
        {cx + sepX*3,   baseY + sepY/2},
        {cx + sepX*3,   baseY + sepY*1.5}
    };

    totemsRestantes = pos.size();
    for (auto &p : pos) {
        Totems *t = new Totems();
        t->setPos(p.x, p.y);
        t->setZValue(3);
        listaTotems.append(t);
        scene->addItem(t);
    }

    configurarFisica();
}

void NivelCueva::configurarFisica() {
    if (roca) roca->usarGravedad = false;
}
void NivelCueva::actualizarFisica() {
    if (roca && roca->y() > SY-RH) roca->setY(SY-RH);
}
void NivelCueva::avance(int fase) {
    if (fase==1 && pedro) pedro->avance(1);
}