#include "dino.h"
#include <cmath>
#include <QRandomGenerator>
#include <QWidget>
#include <QPushButton>
#include <Qvector>
#include <QRect>


Dino::Dino(Roca *r, QObject *parent)
    : Personaje(parent), roca(r)
{
    velocidad     = VEL_MAX;
    reaccionando  = false;
    yaGolpeo      = false;

    saltando      = false;
    velSalto      = 0;
    yBase         = 0;

    xPatrulla     = 520;
    dirP          = 1;

    animD         = D_IDLE;
    audioMusica = new QAudioOutput(this);
    audioFX     = new QAudioOutput(this);

    audioMusica->setVolume(0.25);
    audioFX->setVolume(0.80);
    sndRugido = new QMediaPlayer(this);
    sndRugido->setAudioOutput(audioFX);
    sndRugido->setSource(
        QUrl("qrc:/sonidos/rugido.mp3"));



    QPixmap img(":/imagenes/dino.png");

    if(!img.isNull())
    {
        setPixmap(img.scaled(
            96,
            96,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
    }
    else
    {
        fallbackD();
    }
    tTemp = new QTimer(this);
    tTemp->setSingleShot(true);

    connect(tTemp,
            &QTimer::timeout,
            this,
            &Dino::idleD);

    playD(D_IDLE, true);


    tReaccion = new QTimer(this);
    tReaccion->setSingleShot(true);

    connect(tReaccion, &QTimer::timeout, this, &Dino::actuar);

    tIA = new QTimer(this);

    connect(tIA, &QTimer::timeout, this, &Dino::tickIA);

    tIA->start(16);
}

void Dino::fallbackD()
{
    QPixmap img(":/imagenes/dino.png");

    if(!img.isNull())
    {
        setPixmap(img.scaled(
            96,
            96,
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
    }
    else
    {
        QPixmap fb(96,96);
        fb.fill(QColor(40,160,60));
        setPixmap(fb);
    }
}



void Dino::playD(AnimD a, bool loop)
{
    animD = a;

    if(!loop)
        tTemp->start(500);
    else
        tTemp->stop();
}


void Dino::idleD() {

    if(roca && roca->estaActiva && reaccionando) {
        return;
    }
    playD(D_IDLE, true);
}

void Dino::tickIA() {
    if (!roca) return;
    if (!roca->estaActiva) {
        xPatrulla += dirP * 0.5f;
        if(xPatrulla > 680 || xPatrulla < 400) dirP *= -1;
        setPos(xPatrulla, y());
        if(animD != D_IDLE) playD(D_IDLE, true);
        yaGolpeo = false; reaccionando = false;
        return;
    }
    percibir();
}

void Dino::percibir() {
    float dx   = roca->x() - x();
    float dy   = roca->y() - y();
    float dist = std::sqrt(dx*dx + dy*dy);
    if(roca->estaActiva && dist < DIST_VER && !yaGolpeo && !reaccionando) {
        reaccionando = true;
        sndRugido->setPosition(0);
        sndRugido->play();
        tReaccion->start(RETARDO_MS);
    }
}

void Dino::actuar() {
    if(!roca || !roca->estaActiva) { reaccionando = false; return; }
    double roll = QRandomGenerator::global()->generateDouble();
    if(roll > PROB_EXITO) {
        playD(D_WALK, false);
        reaccionando = false; return;
    }
    float dx = roca->x() - x();
    float dist = std::abs(dx);
    if(dist <= DIST_GOLPE) golpear();
    else                   correr();
    reaccionando = false;
    QTimer::singleShot(1600, this, [=](){ yaGolpeo = false; reaccionando = false; });
}

void Dino::correr()
{
    float dx = roca->x() - x();
    float dir = (dx > 0) ? 1.0f : -1.0f;

    if(dir > 0)
        setScale(1.0);
    else
        setScale(-1.0);

    QTimer *burst = new QTimer(this);

    int n = 0;

    connect(
        burst,
        &QTimer::timeout,
        this,
        [=]() mutable {

            setPos(x() + dir * VEL_MAX, y());

            n++;

            if(n > 35 || !roca->estaActiva)
            {
                burst->stop();
                burst->deleteLater();
            }
        });

    burst->start(16);
}

void Dino::golpear() {
    yaGolpeo = true;
    playD(D_ATTACK, false);
    float red = 0.35f + (float)QRandomGenerator::global()->generateDouble() * 0.30f;
    roca->cambiarVelocidad(roca->velocidadX * (1.0f - red));
    if(roca->usarGravedad) roca->velocidadY -= 2.0f;
}


void Dino::mover()  { percibir(); }
void Dino::avance(int) {}