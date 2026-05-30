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
    Pedro  = nullptr;
}

void NivelCueva::cargarNivel()
{
    scene->clear();
    listaTotems.clear();

    QPixmap fondo(":/imagenes/fondo.png");
    if (!fondo.isNull())
        scene->setBackgroundBrush(fondo.scaled(1000, 600));
    else
        scene->setBackgroundBrush(QBrush(QColor(80, 50, 20)));

    crearSuelo();

    roca = new Roca();
    roca->setPos(120, 455);
    scene->addItem(roca);

    Pedro = new PedroPicapiedra();
    Pedro->setPos(0, 375);
    Pedro->rocaRef = roca;
    scene->addItem(fred);

    colocarTotems();

    configurarFisica();
}

void NivelCueva::crearSuelo()
{
    QGraphicsRectItem *suelo = scene->addRect(0, 500, 1000, 100,
                                              QPen(Qt::NoPen),
                                              QBrush(QColor(120, 80, 40)));
    Q_UNUSED(suelo);

    for (int i = 0; i < 10; i++) {
        QGraphicsLineItem *linea = scene->addLine(
            i * 100, 510, i * 100 + 60, 510,
            QPen(QColor(100, 60, 20), 2, Qt::DashLine));
        Q_UNUSED(linea);
    }
}

void NivelCueva::colocarPinos()
{
    // Formación triangular de 5
    QVector<QPointF> posiciones = {

        {700, 390},
        {670, 340}, {730, 340},
        {640, 290}, {760, 290}
    };

    totemsRestantes = posiciones.size();

    for (const QPointF &pos : posiciones) {
        Totems *totems = new totems();
        totems->setPos(pos);
        listaTotems.append(totems);
        scene->addItem(totems);
    }
}

void NivelCueva::configurarFisica()
{
    if (roca) roca->usarGravedad = false;
}

void NivelCueva::actualizarFisica()
{
    if (roca && roca->y() > 455) {
        roca->setY(455);
        roca->velocidadY = 0;
    }
}

void NivelCueva::avance(int fase)
{
    Q_UNUSED(fase);
}