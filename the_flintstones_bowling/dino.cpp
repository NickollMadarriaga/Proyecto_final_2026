#include "dino.h"
#include <cmath>
#include <QRandomGenerator>

const int Dino::SF[4] = {4,6,5,4};

Dino::Dino(Roca *r, QObject *parent) : Personaje(parent), roca(r) {
    velocidad=VEL_MAX; reaccionando=false; yaGolpeo=false;
    saltando=false; velSalto=0; yBase=0; xPatrulla=520; dirP=1;
    animD=D_IDLE; frameD=0; loopD=true; sheetOk=false;

    sheet = QPixmap(":/imagenes/dino_sprites.png");
    if (!sheet.isNull()) sheetOk=true;
    else fallbackD();

    tSprite=new QTimer(this); tSprite->setInterval(SFPS);
    connect(tSprite,&QTimer::timeout,this,&Dino::nextFrameD);
    tTemp=new QTimer(this); tTemp->setSingleShot(true);
    connect(tTemp,&QTimer::timeout,this,&Dino::idleD);
    if(sheetOk) loadFrame();
    playD(D_IDLE,true);

    tReaccion=new QTimer(this); tReaccion->setSingleShot(true);
    connect(tReaccion,&QTimer::timeout,this,&Dino::actuar);

    tIA=new QTimer(this);
    connect(tIA,&QTimer::timeout,this,&Dino::tickIA);
    tIA->start(16);
}

void Dino::fallbackD() {
    QPixmap img(":/imagenes/dino.png");
    if (!img.isNull()) setPixmap(img.scaled(100,100,Qt::KeepAspectRatio));
    else { QPixmap fb(100,100); fb.fill(QColor(40,160,60)); setPixmap(fb); }
}
void Dino::loadFrame() {
    if (!sheetOk) return;
    setPixmap(sheet.copy(frameD*SW, animD*SH, SW, SH));
}
void Dino::playD(AnimD a, bool loop) {
    animD=a; frameD=0; loopD=loop;
    if(sheetOk) loadFrame();
    tSprite->start(SFPS);
    if(!loop) tTemp->start(SF[a]*SFPS); else tTemp->stop();
}
void Dino::nextFrameD() {
    if(!sheetOk) return;
    frameD++;
    if(frameD>=SF[animD]) { if(loopD) frameD=0; else{frameD=SF[animD]-1;tSprite->stop();return;} }
    loadFrame();
}
void Dino::idleD() { playD(D_IDLE,true); }

// ── IA ────────────────────────────────────────────────────────
void Dino::tickIA() {
    if (!roca) return;
    if (!roca->estaActiva) {
        // Patrullar suavemente
        xPatrulla += dirP * 0.5f;
        if (xPatrulla>680||xPatrulla<400) dirP*=-1;
        setPos(xPatrulla, y());
        playD(D_IDLE,true);
        yaGolpeo=false; reaccionando=false;
        return;
    }
    percibir();
}

void Dino::percibir() {
    float dx   = roca->x()-x();
    float dy   = roca->y()-y();
    float dist = std::sqrt(dx*dx+dy*dy);
    // Solo reacciona si la roca viene hacia él
    if (roca->velocidadX > 0.5f && dist < DIST_VER && !yaGolpeo && !reaccionando) {
        reaccionando=true;
        tReaccion->start(RETARDO_MS);
    }
}

void Dino::actuar() {
    if (!roca||!roca->estaActiva) { reaccionando=false; return; }

    // Lanzar moneda: ¿logra interceptar?
    double roll = QRandomGenerator::global()->generateDouble();
    if (roll > PROB_EXITO) {
        // Falla: Dino se mueve pero no alcanza
        playD(D_CORRER,false);
        reaccionando=false; return;
    }

    float dx   = roca->x()-x();
    float dist = std::abs(dx);

    if (dist<=DIST_GOLPE)        golpear();
    else if (dist<=DIST_CORRER)  correr();
    else                         correr();

    reaccionando=false;
    QTimer::singleShot(1600,this,[=](){yaGolpeo=false; reaccionando=false;});
}

void Dino::correr() {
    float dx  = roca->x()-x();
    float dir = (dx>0)?1.0f:-1.0f;
    playD(D_CORRER,true);
    // Mover en ráfagas
    QTimer *burst=new QTimer(this); int n=0;
    connect(burst,&QTimer::timeout,this,[=]()mutable{
        setPos(x()+dir*VEL_MAX, y()); n++;
        if(n>35||!roca->estaActiva) burst->stop();
    });
    burst->start(16);
}

void Dino::saltar() {
    if(saltando) return;
    saltando=true; yBase=y(); velSalto=-8.5f;
    playD(D_SALTAR,false);
    QTimer *ts=new QTimer(this);
    connect(ts,&QTimer::timeout,this,[=](){
        tickSalto(); if(!saltando){ts->stop();ts->deleteLater();}
    });
    ts->start(16);
}
void Dino::tickSalto() {
    velSalto+=0.48f; setPos(x(),y()+velSalto);
    if(y()>=yBase){setPos(x(),yBase);saltando=false;velSalto=0;playD(D_IDLE,true);}
}

void Dino::golpear() {
    yaGolpeo=true;
    playD(D_ATACAR,false);
    // Reduce velocidad entre 35-65% (no para en seco)
    float red = 0.35f + (float)QRandomGenerator::global()->generateDouble()*0.30f;
    roca->cambiarVelocidad(roca->velocidadX*(1.0f-red));
    if(roca->usarGravedad) roca->velocidadY -= 2.0f;
}

void Dino::mover()  { percibir(); }
void Dino::avance(int) {}