#ifndef DINO_H
#define DINO_H
#include "personaje.h"
#include "roca.h"
#include <QTimer>

// IA: Percepción → Decisión → Acción
// Dificultad balanceada: reacciona con 380ms de retardo,
// intercepción exitosa ~60%, velocidad limitada a 2.6px/tick

class Dino : public Personaje {
    Q_OBJECT
public:
    explicit Dino(Roca *roca, QObject *parent=nullptr);
    void mover() override;
    void avance(int) override;

    enum AnimD { D_IDLE,D_CORRER,D_SALTAR,D_ATACAR };
    void playD(AnimD a, bool loop=true);

private:
    Roca   *roca;
    QTimer *tIA, *tSprite, *tTemp, *tReaccion;

    bool  reaccionando, yaGolpeo, saltando;
    float velSalto, yBase, xPatrulla;
    int   dirP;

    // Spritesheet :/imagenes/dino_sprites.png (opcional)
    QPixmap sheet;
    bool    sheetOk;
    static const int SW=100,SH=100,SFPS=85;
    static const int SF[4]; // frames por animación
    AnimD animD; int frameD; bool loopD;
    void loadFrame();
    void fallbackD();

    static constexpr float PROB_EXITO    = 0.60f;
    static constexpr float DIST_VER      = 340.0f;
    static constexpr float DIST_CORRER   = 170.0f;
    static constexpr float DIST_GOLPE    = 72.0f;
    static constexpr float VEL_MAX       = 2.6f;
    static constexpr int   RETARDO_MS    = 380;

    void percibir();
    void decidir(float dist, float dx);
    void correr();
    void saltar();
    void golpear();

private slots:
    void tickIA();
    void actuar();
    void nextFrameD();
    void idleD();
    void tickSalto();
};
#endif