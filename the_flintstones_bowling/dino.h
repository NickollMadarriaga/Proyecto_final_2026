#ifndef DINO_H
#define DINO_H

#include "personaje.h"
#include "roca.h"

#include <QTimer>
#include <QPixmap>
#include <QObject>

class Dino : public Personaje
{
    Q_OBJECT

public:
    explicit Dino(Roca *roca, QObject *parent = nullptr);

    void mover() override;
    void avance(int) override;
    void congelar();

    enum AnimD {
        D_IDLE,
        D_WALK,
        D_ATTACK,
        D_FROZEN
    };

    void playD(AnimD a, bool loop = true);

private:
    Roca *roca;

    QTimer *tIA;
    QTimer *tTemp;
    QTimer *tReaccion;

    bool reaccionando;
    bool yaGolpeo;
    bool saltando;

    float velSalto;
    float yBase;
    float xPatrulla;

    int dirP;

    AnimD animD;

    void fallbackD();

    static constexpr float PROB_EXITO = 0.60f;
    static constexpr float DIST_VER   = 340.0f;
    static constexpr float DIST_GOLPE = 72.0f;
    static constexpr float VEL_MAX    = 2.6f;

    static constexpr int RETARDO_MS = 380;

    void percibir();
    void correr();
    void golpear();

private slots:
    void tickIA();
    void actuar();
    void idleD();
};

#endif