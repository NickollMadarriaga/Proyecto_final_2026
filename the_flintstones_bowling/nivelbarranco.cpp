#include "nivelbarranco.h"
#include <QPixmap>
#include <QPen>
#include <QBrush>
#include <QColor>
#include <QTimer>

NivelBarranco::NivelBarranco(QGraphicsScene *scene)
    : Nivel(scene)
{
    nombre   = "Nivel 2 - El Barranco";
    gravedad = 0.4f;
    Pedro    = nullptr;
    dino     = nullptr;
}

void NivelBarranco::cargarNivel()
{
    scene->clear();
    listaTotems.clear();
    plataformas.clear();

    QPixmap fondo(":/imagenes/fondo2.png");
    if (!fondo.isNull())
        scene->setBackgroundBrush(fondo.scaled(1000, 600));
    else
        scene->setBackgroundBrush(QBrush(QColor(50, 80, 120)));

    QGraphicsRectItem *suelo = scene->addRect(0, 550, 1000, 50,
                                              QPen(Qt::NoPen), QBrush(QColor(80, 60, 30)));
    Q_UNUSED(suelo);

    crearPlataformas();

    roca = new Roca();
    roca->setPos(50, 460);
    scene->addItem(roca);

    Pedro = new Pedropicapiedra();
    Pedro->setPos(0, 370);
    Pedro->rocaRef = roca;
    scene->addItem(Pedro);

    colocarTotems();

    dino = new Dino(roca);
    dino->setPos(550, 460);
    scene->addItem(dino);

    configurarFisica();
}

void NivelBarranco::crearPlataformas()
{
    // Plataformas a diferentes alturas según el nivel
    struct PlataformaInfo { int x, y, w, h; };
    QVector<PlataformaInfo> infos = {
        {300, 430, 180, 18},   // Plataforma baja-izquierda
        {550, 340, 160, 18},   // Plataforma media
        {750, 240, 160, 18},   // Plataforma alta
        {420, 250, 120, 18},   // Plataforma extra media-alta
    };

    for (auto &info : infos) {
        QGraphicsRectItem *plat = scene->addRect(
            info.x, info.y, info.w, info.h,
            QPen(QColor(60, 40, 10), 2),
            QBrush(QColor(110, 75, 35)));
        plataformas.append(plat);
    }
}

void NivelBarranco::colocarTotems()
{
    QVector<QPointF> posiciones = {
        {330,  305},
        {460,  405},
        {590,  215},
        {780,  115},
        {440,  125},
    };

    totemsRestantes = posiciones.size();

    for (const QPointF &pos : posiciones) {
        Totems *totems = new Totems();
        totems->setPos(pos);
        listaTotems.append(totems);
        scene->addItem(totems);
    }
}

void NivelBarranco::configurarFisica()
{

    if (roca) roca->usarGravedad = true;
}

void NivelBarranco::actualizarFisica()
{
    if (!roca || !roca->estaActiva) return;

    for (QGraphicsRectItem *plat : plataformas) {
        QRectF rTotems = roca->mapToScene(roca->boundingRect()).boundingRect();
        QRectF rPlat = plat->sceneBoundingRect();

        if (rTotems.intersects(rPlat) && roca->velocidadY > 0) {
            roca->setY(rPlat.top() - roca->boundingRect().height());
            roca->velocidadY = 0;
            roca->velocidadX *= 0.985f;
        }
    }
}

void NivelBarranco::avance(int fase)
{
    Q_UNUSED(fase);
}