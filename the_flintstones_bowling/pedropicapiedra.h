#ifndef PEDROPICAPIEDRA_H
#define PEDROPICAPIEDRA_H
#include "personaje.h"
#include "roca.h"
#include "indicadorpunteria.h"
#include <QVector>
#include <QTimer>

// Sprites individuales: :/imagenes/fred/N.png
// IDLE:     1-5   CELEBRAR: 6-9
// GRITO:   29-51 + 100-103
// LANZAR:  52-83  CAMINAR: 84-92

class PedroPicapiedra : public Personaje {
    Q_OBJECT
public:
    explicit PedroPicapiedra(QObject *parent = nullptr);
    void mover() override;
    void avance(int fase) override;
    void cargarFuerza(float v);
    void lanzarRoca();

    // Habilidad 1 — máximo 1 uso por nivel
    void gritoGuerra();
    bool gritoDisponible;

    // Habilidad 2 — máximo 1 uso por nivel
    void activarPunteria();
    void desactivarPunteria();
    void actualizarPunteria(bool gravedad=false);
    bool pusteriaDisponible;
    bool pusteriaActiva;

    float fuerzaCargada;
    Roca *rocaRef;
    IndicadorPunteria *indicadorPunteria;

    enum Anim { IDLE,CELEBRAR,GRITO,LANZAR,CAMINAR };
    void play(Anim a, bool loop=true);

private:
    static const int FPS = 40;
    QVector<QPixmap> fIdle,fCelebrar,fGrito,fLanzar,fCaminar;
    QVector<QPixmap> *cur;
    Anim   animActual;
    int    frame;
    bool   looping;
    bool   cargado;
    QTimer *tSprite, *tTemp;

    void loadRange(QVector<QPixmap>&, QVector<int>);
    void fallback();
    void showFrame();

private slots:
    void nextFrame();
    void backIdle();
};
#endif //PEDROPICAPIEDRA_H