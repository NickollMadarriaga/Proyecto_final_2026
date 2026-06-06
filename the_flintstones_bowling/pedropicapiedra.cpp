#include "pedropicapiedra.h"
#include "roca.h"

PedroPicapiedra::PedroPicapiedra(QObject *parent) : Personaje(parent) {
    fuerzaCargada    = 9.0f;
    rocaRef          = nullptr;
    animActual       = IDLE;
    frame            = 0;
    looping          = true;
    cargado          = false;
    cur              = &fIdle;
    gritoDisponible    = true;
    pusteriaDisponible = true;

    loadRange(fIdle,     {1,2,3,4,5});
    loadRange(fCelebrar,{6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22});
    {
        QVector<int> g;
        for (int i=29; i<=51; i++) g<<i;
        g<<100<<101<<102<<103;
        loadRange(fGrito, g);
    }
    {
        QVector<int> l;
        for (int i=30; i<=83; i++) l<<i;
        loadRange(fLanzar, l);
    }
    {
        QVector<int> c;
        for (int i=84; i<=92; i++) c<<i;
        loadRange(fCaminar, c);
    }

    if (!cargado) fallback();

    tSprite = new QTimer(this);
    tSprite->setInterval(FPS);
    connect(tSprite, &QTimer::timeout, this, &PedroPicapiedra::nextFrame);

    tTemp = new QTimer(this);
    tTemp->setSingleShot(true);
    connect(tTemp, &QTimer::timeout, this, &PedroPicapiedra::backIdle);

    play(IDLE, true);
    audioMusica = new QAudioOutput(this);
    audioFX     = new QAudioOutput(this);

    audioMusica->setVolume(0.25);
    audioFX->setVolume(0.80);
    sndGrito = new QMediaPlayer(this);
    sndGrito->setAudioOutput(audioFX);
    sndGrito->setSource(
        QUrl("qrc:/sonidos/grito.mp3"));

}

void PedroPicapiedra::loadRange(QVector<QPixmap> &v, QVector<int> nums) {
    for (int n : nums) {
        QPixmap px(QString(":/imagenes/%1.png").arg(n));
        if (!px.isNull()) {
            v << px.scaled(201,151,Qt::KeepAspectRatio,Qt::SmoothTransformation);
            cargado = true;
        }
    }
    if (v.isEmpty()) {
        QPixmap fb(201,151); fb.fill(Qt::transparent); v << fb;
    }
}

void PedroPicapiedra::fallback() {
    QPixmap img(":/imagenes/fred.png");
    if (!img.isNull())
        setPixmap(img.scaled(201,151,Qt::KeepAspectRatio));
    else {
        QPixmap fb(120,151); fb.fill(Qt::blue); setPixmap(fb);
    }
}

void PedroPicapiedra::showFrame() {
    if (!cur || cur->isEmpty()) return;
    frame = qBound(0, frame, cur->size()-1);
    setPixmap(cur->at(frame));
}

void PedroPicapiedra::play(Anim a, bool loop) {
    animActual = a; frame = 0; looping = loop;
    switch (a) {
    case IDLE:     cur = &fIdle;     break;
    case CELEBRAR: cur = &fCelebrar; break;
    case GRITO:    cur = &fGrito;    break;
    case LANZAR:   cur = &fLanzar;   break;
    case CAMINAR:  cur = &fCaminar;  break;
    }
    showFrame();
    tSprite->start(FPS);
    if (!loop) tTemp->start(cur->size() * FPS);
    else       tTemp->stop();
}

void PedroPicapiedra::nextFrame() {
    if (!cur || cur->isEmpty()) return;

    frame++;

    if(animActual == LANZAR && frame == 46)
    {
        emit soltarRoca();
    }

    if (frame >= cur->size()) {
        if (looping) frame = 0;
        else {
            frame = cur->size()-1;
            tSprite->stop();
            return;
        }
    }

    showFrame();
}

void PedroPicapiedra::backIdle() { play(IDLE, true); }

void PedroPicapiedra::gritoGuerra() {
    if (!gritoDisponible) return;
    gritoDisponible = false;
    if (rocaRef) rocaRef->activarSuperRoca();
    play(GRITO, false);
    sndGrito->setPosition(0);
    sndGrito->play();
}

void PedroPicapiedra::lanzarRoca() {
    play(LANZAR, false);
}

void PedroPicapiedra::cargarFuerza(float v) {
    fuerzaCargada = qBound(3.0f, fuerzaCargada + v, 20.0f);
}

void PedroPicapiedra::mover() {}

void PedroPicapiedra::avance(int fase) {
    if (fase == 1) play(CELEBRAR, false);
}