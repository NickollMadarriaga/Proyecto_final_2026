#include "nivelcueva.h"
#include <QPixmap>
#include <QPen>
#include <QBrush>
#include <QColor>

NivelCueva::NivelCueva(QGraphicsScene *scene)
    : Nivel(scene)
{
    nombre   = "Nivel 1 - La Cueva";
    friccion = 0.992f;
    pistaDir = "derecha";
    Pedro     = nullptr;
}

void NivelCueva::cargarNivel()
{
    scene->clear();
    listaTotems.clear();

    QPixmap fondo(":/imagenes/fondo.png");
    if (!fondo.isNull())
        scene->setBackgroundBrush(fondo.scaled(1000, 600,
                                               Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    else
        scene->setBackgroundBrush(QBrush(QColor(180, 160, 120)));

    crearSuelo();


    roca = new Roca();
    roca->setPos(100, 430);
    scene->addItem(roca);


    Pedro = new Pedropicapiedra();
    Pedro->setPos(-20, 339);
    Pedro->rocaRef = roca;
    scene->addItem(Pedro);

    colocarTotems();
    configurarFisica();
}

void NivelCueva::crearSuelo()
{

    scene->addRect(0, 490, 1000, 110,
                   QPen(Qt::NoPen),
                   QBrush(QColor(140, 100, 55, 180)));

    for (int i = 1; i < 10; i++) {
        scene->addLine(i * 100, 495, i * 100 + 60, 495,
                       QPen(QColor(110, 75, 35, 150), 2, Qt::DashLine));
    }
}

void NivelCueva::colocarTotems()
{

    QVector<QPointF> posiciones = {

                                   {750, 378},
                                   {710, 338}, {790, 338},
                                   {670, 298}, {830, 298},
                                   };

    totemsRestantes = posiciones.size();

    for (const QPointF &pos : posiciones) {
        Totems *totems = new Totems();
        QPixmap img(":/imagenes/Totem.png");
        if (!img.isNull())
            totems->setPixmap(img.scaled(55, 110,
                                       Qt::KeepAspectRatio, Qt::SmoothTransformation));
        totems->setPos(pos);
        totems->setTransformOriginPoint(27, 110); // pivote en la base
        listaTotems.append(totems);
        scene->addItem(totems);
    }
}

void NivelCueva::configurarFisica()
{
    // Nivel 1: solo fricción horizontal, sin gravedad
    if (roca) {
        roca->usarGravedad = false;
    }
}

void NivelCueva::actualizarFisica()
{
    if (roca && roca->y() > 430) {
        roca->setY(430);
        roca->velocidadY = 0;
    }
}

void NivelCueva::avance(int fase) { Q_UNUSED(fase); }