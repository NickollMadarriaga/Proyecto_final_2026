#include "dino.h"
#include <cmath>


constexpr int Dino::FRAMES_POR_FILA[5];

Dino::Dino(Roca *rocaRef, QObject *parent)
    : Personaje(parent), roca(rocaRef)
{
    velocidad      = 2.5f;
    yaGolpeo       = false;
    saltando       = false;
    velocidadSalto = 0;
    alturaOriginal = 0;
    animActual     = IDLE;
    frameActual    = 0;
    loopActivo     = true;
    spriteCargado  = false;

    spritesheet = QPixmap(":/imagenes/dino_sprites.png");
    if (!spritesheet.isNull()) {
        spriteCargado = true;
    } else {
        cargarFallback();
    }

    timerSprite = new QTimer(this);
    timerSprite->setInterval(FPS_ANIMACION);
    connect(timerSprite, &QTimer::timeout, this, &Dino::avanzarFrame);

    timerAnimTemporal = new QTimer(this);
    timerAnimTemporal->setSingleShot(true);
    connect(timerAnimTemporal, &QTimer::timeout,
            this, &Dino::onAnimacionTerminada);

    if (spriteCargado) cargarFrame();
    reproducirAnimacion(IDLE, true);

    timerIA = new QTimer(this);
    connect(timerIA, &QTimer::timeout, this, &Dino::actualizarIA);
    timerIA->start(16);
}

void Dino::cargarFrame()
{
    if (!spriteCargado) return;

    int x = frameActual * FRAME_W;
    int y = animActual  * FRAME_H;

    QPixmap frame = spritesheet.copy(x, y, FRAME_W, FRAME_H);
    setPixmap(frame);
}

void Dino::cargarFallback()
{
    QPixmap img(":/imagenes/dino.png");
    if (!img.isNull())
        setPixmap(img.scaled(100, 100, Qt::KeepAspectRatio));
    else {
        QPixmap fb(100, 100);
        fb.fill(Qt::green);
        setPixmap(fb);
    }
}

void Dino::reproducirAnimacion(Animacion anim, bool loop)
{
    if (!spriteCargado) return;

    if (!loop && (animActual == ATACAR || animActual == SALTAR)
        && anim != ATACAR && anim != SALTAR) return;

    animActual  = anim;
    frameActual = 0;
    loopActivo  = loop;

    cargarFrame();
    timerSprite->start(FPS_ANIMACION);

    if (!loop) {
        int duracion = FRAMES_POR_FILA[anim] * FPS_ANIMACION;
        timerAnimTemporal->start(duracion);
    } else {
        timerAnimTemporal->stop();
    }
}

void Dino::avanzarFrame()
{
    if (!spriteCargado) return;

    frameActual++;
    int totalFrames = FRAMES_POR_FILA[animActual];

    if (frameActual >= totalFrames) {
        if (loopActivo) {
            frameActual = 0;
        } else {
            frameActual = totalFrames - 1;
            timerSprite->stop();
            return;
        }
    }

    cargarFrame();
}

void Dino::onAnimacionTerminada()
{
    reproducirAnimacion(IDLE, true);
}

void Dino::percibir()
{
    if (!roca || !roca->estaActiva) {
        reproducirAnimacion(IDLE, true);
        return;
    }

    float dx = roca->x() - x();
    float dy = roca->y() - y();
    float distancia = std::sqrt(dx*dx + dy*dy);

    decidir(distancia, dx, dy);
}

void Dino::decidir(float distancia, float dx, float dy)
{
    Q_UNUSED(dy);

    if (distancia > DISTANCIA_ALERTA) {

        reproducirAnimacion(IDLE, true);
        return;
    }

    if (distancia <= DISTANCIA_GOLPE && !yaGolpeo) {
        desviarConCola();
        return;
    }

    if (distancia <= DISTANCIA_ACCION) {
        if (!saltando && roca->velocidadX > 6.0f) {
            saltar();
        } else {
            correrRapido();
        }
        return;
    }


    reproducirAnimacion(ALERTA, false);
    correrRapido();
}

void Dino::correrRapido()
{
    float dx = roca->x() - x();

    if (std::abs(dx) > DISTANCIA_GOLPE + 20) {
        float dir = (dx > 0) ? 1.0f : -1.0f;
        setPos(x() + dir * velocidad, y());

        if (spriteCargado) {
            QPixmap frame = spritesheet.copy(
                frameActual * FRAME_W, CAMINAR * FRAME_H, FRAME_W, FRAME_H);
            if (dir < 0)
                frame = frame.transformed(QTransform().scale(-1, 1));
            setPixmap(frame);
        }

        reproducirAnimacion(CAMINAR, true);
    }
}

void Dino::saltar()
{
    if (saltando) return;
    saltando = true;
    alturaOriginal = y();
    velocidadSalto = -8.0f;

    reproducirAnimacion(SALTAR, false);

    QTimer *timerSalto = new QTimer(this);
    connect(timerSalto, &QTimer::timeout, this, [=]() {
        actualizarSalto();
        if (!saltando) {
            timerSalto->stop();
            timerSalto->deleteLater();
        }
    });
    timerSalto->start(16);
}

void Dino::actualizarSalto()
{
    velocidadSalto += 0.5f;
    setPos(x(), y() + velocidadSalto);

    if (y() >= alturaOriginal) {
        setPos(x(), alturaOriginal);
        saltando = false;
        velocidadSalto = 0;
        reproducirAnimacion(IDLE, true);
    }
}

void Dino::desviarConCola()
{
    yaGolpeo = true;
    reproducirAnimacion(ATACAR, false);

    float nuevaVel = -(roca->velocidadX * 0.6f);
    roca->cambiarVelocidad(nuevaVel);

    if (roca->usarGravedad)
        roca->velocidadY -= 3.0f;

    QTimer::singleShot(2000, this, [=]() { yaGolpeo = false; });
}

void Dino::mover()
{
    percibir();
}

void Dino::avance(int fase)
{
    Q_UNUSED(fase);
}

void Dino::actualizarIA()
{
    if (roca && roca->estaActiva)
        percibir();
}