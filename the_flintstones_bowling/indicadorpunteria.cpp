#include "indicadorpunteria.h"
#include <QPen>
#include <QBrush>
#include <QPolygonF>
#include <QGraphicsScene>

IndicadorPunteria::IndicadorPunteria(QObject *parent)
    : QObject(parent), QGraphicsItemGroup()
{
    visible = false;
    flecha = nullptr;

    for (int i = 0; i < 8; i++) {
        QGraphicsEllipseItem *punto = new QGraphicsEllipseItem(-5, -5, 10, 10, this);
        int alpha = 255 - (i * 28);
        punto->setBrush(QBrush(QColor(255, 200, 50, alpha)));
        punto->setPen(QPen(QColor(200, 100, 0, alpha), 1));
        puntos.append(punto);
    }

    QPolygonF poligono;
    poligono << QPointF(0, -8) << QPointF(6, 4) << QPointF(-6, 4);
    flecha = new QGraphicsPolygonItem(poligono, this);
    flecha->setBrush(QBrush(QColor(255, 100, 0, 200)));
    flecha->setPen(QPen(Qt::transparent));

    setVisible(false);
}

void IndicadorPunteria::mostrar(float px, float py, float fuerza, bool usarGravedad)
{
    actualizar(px, py, fuerza, usarGravedad);
    setVisible(true);
    visible = true;
}

void IndicadorPunteria::ocultar()
{
    setVisible(false);
    visible = false;
}

void IndicadorPunteria::actualizar(float px, float py, float fuerza, bool usarGravedad)
{
    float vx = fuerza;
    float vy = usarGravedad ? -fuerza * 0.8f : 0;
    float grav = usarGravedad ? 0.4f : 0;

    float simX = px + 30;
    float simY = py + 30;

    for (int i = 0; i < puntos.size(); i++) {
        for (int paso = 0; paso < 10; paso++) {
            vy += grav;
            simX += vx;
            simY += vy;
            if (!usarGravedad) vx *= 0.992f;
        }
        puntos[i]->setPos(simX, simY);
    }

    if (puntos.size() >= 2) {
        QPointF p1 = puntos[puntos.size()-2]->pos();
        QPointF p2 = puntos[puntos.size()-1]->pos();
        float angulo = std::atan2(p2.y() - p1.y(), p2.x() - p1.x()) * 180.0 / M_PI + 90;
        flecha->setPos(p2);
        flecha->setRotation(angulo);
    }
}