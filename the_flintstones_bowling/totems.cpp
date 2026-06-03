#include "totems.h"
#include <QPixmap>

Totems::Totems(QObject *parent) : QObject(parent), QGraphicsPixmapItem() {
    derribado = false;
    angulo    = 0;

    QPixmap img(":/imagenes/Totem.png");
    if (!img.isNull())
        // Más grandes: 60×120 para que sean bien visibles
        setPixmap(img.scaled(60,120,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    else {
        QPixmap fb(60,120); fb.fill(QColor(100,70,35)); setPixmap(fb);
    }

    setTransformOriginPoint(30, 120); // pivote en la base para caída realista

    timerAnim = new QTimer(this);
    connect(timerAnim,&QTimer::timeout,this,&Totems::tickAnimacion);
}

void Totems::derribar() {
    if (derribado) return;
    derribado = true;
    timerAnim->start(16);
}

void Totems::tickAnimacion() {
    angulo += 5.0f;
    setRotation(angulo);
    if (angulo >= 90.0f) {
        setRotation(90);
        timerAnim->stop();
    }
}