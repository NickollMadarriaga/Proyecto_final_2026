#ifndef DINO_H
#define DINO_H

#include "personaje.h"
#include "roca.h"
#include <QPixmap>
#include <QTimer>
#include <QObject>

class Dino : public Personaje
{
    Q_OBJECT
public:
    explicit Dino(Roca *rocaRef, QObject *parent = nullptr);

    void mover() override;
    void avance(int fase) override;

    void percibir();
    void correrRapido();
    void saltar();
    void desviarConCola();


    enum Animacion {
        IDLE    = 0,
        CAMINAR = 1,
        SALTAR  = 2,
        ATACAR  = 3,
        ALERTA  = 4
    };

    void reproducirAnimacion(Animacion anim, bool loop = true);

private:

    static constexpr int FRAME_W       = 100;
    static constexpr int FRAME_H       = 100;
    static constexpr int FPS_ANIMACION = 90;

    static constexpr int FRAMES_POR_FILA[5] = {4, 6, 5, 4, 3};


    QPixmap  spritesheet;
    bool     spriteCargado;

    Animacion animActual;
    int       frameActual;
    bool      loopActivo;

    QTimer *timerSprite;
    QTimer *timerAnimTemporal;

    void cargarFrame();
    void cargarFallback();


    Roca  *roca;
    QTimer *timerIA;

    bool  yaGolpeo;
    bool  saltando;
    float velocidadSalto;
    float alturaOriginal;

    static constexpr float DISTANCIA_ALERTA = 300.0f;
    static constexpr float DISTANCIA_ACCION = 150.0f;
    static constexpr float DISTANCIA_GOLPE  = 80.0f;

    void decidir(float distancia, float dx, float dy);

private slots:
    void avanzarFrame();
    void onAnimacionTerminada();
    void actualizarIA();
    void actualizarSalto();
};

#endif // DINO_H