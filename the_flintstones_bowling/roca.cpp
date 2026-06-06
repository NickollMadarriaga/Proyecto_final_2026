#include "roca.h"
#include "totems.h"
#include <QPixmap>
#include <cmath>
#include "pedropicapiedra.h"

Roca::Roca(QObject *parent) : QObject(parent), QGraphicsPixmapItem() {
    velocidadX=0; velocidadY=0; estaActiva=false;
    superRoca=false; usarGravedad=false;
    friccion=0.999f; tamBase=55; sueloY=475;
    tiempo=0; velLanzamiento=0; anguloRad=0;

    setImagenNormal();
    timerH = new QTimer(this);
    timerH->setInterval(16);
    connect(timerH, &QTimer::timeout, this, &Roca::moverHorizontal);
    timerP = new QTimer(this);
    timerP->setInterval(30);
    connect(timerP, &QTimer::timeout, this, &Roca::moverParabolico);
    audioMusica = new QAudioOutput(this);
    audioFX     = new QAudioOutput(this);

    audioMusica->setVolume(0.25);
    audioFX->setVolume(0.80);

    sndColision = new QMediaPlayer(this);
    sndColision->setAudioOutput(audioFX);
    sndColision->setSource(
        QUrl("qrc:/sonidos/colision.mp3"));
}

void Roca::setImagenNormal() {
    QPixmap img(":/imagenes/roca.png");
    if (!img.isNull())
        setPixmap(img.scaled(tamBase,tamBase,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    else { QPixmap fb(tamBase,tamBase); fb.fill(Qt::darkGray); setPixmap(fb); }
}

// NIVEL 1:
void Roca::lanzar(float vx) {
    timerP->stop();
    velocidadX=vx; velocidadY=0;
    usarGravedad=false; estaActiva=true;
    setY(sueloY);  // clavar al suelo
    timerH->start();
}

void Roca::moverHorizontal() {

    int pasos = std::max(1, (int)std::abs(velocidadX));
    float dx = velocidadX / pasos;

    for(int i = 0; i < pasos; i++) {

        setX(x() + dx);
        setY(sueloY);

        detectarColisiones();

        if(!estaActiva)
            return;
    }

    setRotation(rotation() + velocidadX * 3.0f);

    velocidadX *= friccion;

    if (std::abs(velocidadX) < 0.1f) {
        velocidadX = 0;
        estaActiva = false;
        timerH->stop();
        emit rocaDetenida();
        return;
    }

    if (x() > 1060 || x() < -80) {
        estaActiva = false;
        timerH->stop();
        emit rocaDetenida();
        return;
    }
}
// NIVEL 2:
void Roca::lanzarParabolico(PedroPicapiedra* pedro, float anguloGrados) {
    timerH->stop();
    tiempo         = 0;
    float f = pedro->fuerzaCargada;
    velLanzamiento = f*40.0;
    anguloRad      = anguloGrados * M_PI / 180.0;
    origen         = QPointF(x(), y());
    usarGravedad   = true;
    estaActiva     = true;
    timerP->start();
}

void Roca::moverParabolico() {
    tiempo += 0.030;

    const double g = 9.8;
    double vx = velLanzamiento * qCos(anguloRad);
    double vy = -velLanzamiento * qSin(anguloRad);
    double dx = vx * tiempo;
    double dy = vy * tiempo + 0.5 * g * tiempo * tiempo;

    QPointF nuevaPos = origen + QPointF(dx, dy);
    setPos(nuevaPos);
    setRotation(rotation() + 4.0);

    if (nuevaPos.x() > 1060 || nuevaPos.x() < -80 || nuevaPos.y() > 700) {
        estaActiva = false; timerP->stop(); emit rocaDetenida(); return;
    }

    detectarColisiones();
}

void Roca::activarSuperRoca() {
    superRoca=true;
    QPixmap img(":/imagenes/roca.png");
    int sz=85;
    if (!img.isNull())
        setPixmap(img.scaled(sz,sz,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    else { QPixmap fb(sz,sz); fb.fill(QColor(200,80,0)); setPixmap(fb); }
}

void Roca::cambiarVelocidad(float v) {
    velocidadX=v;
    velLanzamiento = std::abs(v) * 2.0;
}

void Roca::resetear(float px, float py) {
    timerH->stop(); timerP->stop();
    velocidadX=0; velocidadY=0; tiempo=0;
    estaActiva=false; superRoca=false; usarGravedad=false;
    setPos(px,py); setRotation(0);
    setImagenNormal();
}

void Roca::detectarColisiones() {
    for (QGraphicsItem *item : collidingItems()) {
        Totems *t = dynamic_cast<Totems*>(item);
        if (t && !t->derribado) {
            t->derribar();
            velocidadX *= superRoca ? 0.95f : 0.80f;
            velLanzamiento *= 0.80;
            emit pinoGolpeado();
            sndColision->setPosition(0);
            sndColision->play();
        }
    }
}