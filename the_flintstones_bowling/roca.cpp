#include "roca.h"
#include "totems.h"
#include <QPixmap>
#include <cmath>

Roca::Roca(QObject *parent) : QObject(parent), QGraphicsPixmapItem() {
    velocidadX=0; velocidadY=0; estaActiva=false;
    superRoca=false; usarGravedad=false;
    gravedad=0.42f; friccion=0.999f; tamBase=55; sueloY=475;

    QPixmap img(":/imagenes/roca.png");
    if (!img.isNull())
        setPixmap(img.scaled(tamBase,tamBase,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    else { QPixmap fb(tamBase,tamBase); fb.fill(Qt::darkGray); setPixmap(fb); }

    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&Roca::mover);
    timer->start(16);
}

void Roca::lanzar(float vx) {
    // Nivel 1: roca horizontal, pegada al suelo, sin ningún componente vertical
    velocidadX  = vx;
    velocidadY  = 0;
    usarGravedad = false;
    estaActiva   = true;
    // Fijar Y exactamente en el suelo al lanzar
    setY(sueloY);
}

void Roca::lanzarParabolico(float vx, float vy) {
    velocidadX=vx; velocidadY=vy; usarGravedad=true; estaActiva=true;
}

void Roca::activarSuperRoca() {
    superRoca=true;
    QPixmap img(":/imagenes/roca.png");
    int sz=85;
    if (!img.isNull())
        setPixmap(img.scaled(sz,sz,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    else { QPixmap fb(sz,sz); fb.fill(QColor(200,80,0)); setPixmap(fb); }
}
void Roca::cambiarVelocidad(float v) { velocidadX=v; }

void Roca::resetear(float px, float py) {
    velocidadX=0; velocidadY=0; estaActiva=false;
    superRoca=false; usarGravedad=false;
    setPos(px,py); setRotation(0);
    QPixmap img(":/imagenes/roca.png");
    if (!img.isNull())
        setPixmap(img.scaled(tamBase,tamBase,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    else { QPixmap fb(tamBase,tamBase); fb.fill(Qt::darkGray); setPixmap(fb); }
}

void Roca::mover() {
    if (!estaActiva) return;

    if (usarGravedad) {
        // Nivel 2: movimiento parabólico real
        velocidadY += gravedad;
        setPos(x()+velocidadX, y()+velocidadY);

        // Colisión suelo nivel 2 — SIN rebote, solo frena
        if (y() >= sueloY) {
            setY(sueloY);
            velocidadY  = 0;
            usarGravedad = false;  // ya tocó suelo, sigue rodando
        }
    } else {
        // Nivel 1: movimiento horizontal puro, Y fija siempre
        setX(x() + velocidadX);
        setY(sueloY);  // clavar Y en el suelo en todo momento
    }

    // Rotación visual de la roca rodando
    setRotation(rotation() + velocidadX * 2.5f);

    // Fricción suave (solo frena muy despacio)
    velocidadX *= friccion;

    // Detener si muy lenta
    if (std::abs(velocidadX) < 0.1f) {
        velocidadX=0; estaActiva=false;
        emit rocaDetenida(); return;
    }

    // Salió de pantalla
    if (x() > 1060 || x() < -80) {
        estaActiva=false; emit rocaDetenida(); return;
    }

    detectarColisiones();
}

void Roca::detectarColisiones() {
    for (QGraphicsItem *item : collidingItems()) {
        Totems *t = dynamic_cast<Totems*>(item);
        if (t && !t->derribado) {
            t->derribar();
            velocidadX *= superRoca ? 0.95f : 0.82f;
            emit pinoGolpeado();
        }
    }
}