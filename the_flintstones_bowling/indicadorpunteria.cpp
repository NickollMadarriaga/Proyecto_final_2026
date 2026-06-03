#include "indicadorpunteria.h"
#include <cmath>
#include <QBrush>
#include <QPen>

IndicadorPunteria::IndicadorPunteria(QObject *parent)
    : QObject(parent), QGraphicsItemGroup() {
    for (int i=0;i<10;i++) {
        int alpha = 240 - i*22;
        int sz = 10 - i/2;
        auto *p = new QGraphicsEllipseItem(-sz/2,-sz/2,sz,sz,this);
        p->setBrush(QBrush(QColor(255,210,50,alpha)));
        p->setPen(QPen(Qt::NoPen));
        puntos.append(p);
    }
    setVisible(false);
}

void IndicadorPunteria::recalcular(float px, float py, float fuerza, bool gravedad) {
    float vx=fuerza, vy = gravedad ? -fuerza*0.75f : 0;
    float sx=px+26, sy=py+26;
    for (auto *p : puntos) {
        for (int s=0;s<12;s++) {
            if (gravedad) vy+=0.42f;
            sx+=vx; sy+=vy;
            if (!gravedad) vx*=0.992f;
        }
        p->setPos(sx,sy);
    }
}
void IndicadorPunteria::mostrar(float px,float py,float f,bool g){
    recalcular(px,py,f,g); setVisible(true);
}
void IndicadorPunteria::actualizar(float px,float py,float f,bool g){
    recalcular(px,py,f,g);
}
void IndicadorPunteria::ocultar(){ setVisible(false); }