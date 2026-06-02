#ifndef PEDROPICAPIEDRA_H
#define PEDROPICAPIEDRA_H

#include "personaje.h"
#include "roca.h"
#include "indicadorpunteria.h"
#include <QPixmap>
#include <QTimer>
#include <QVector>
#include <QGraphicsScene>


class Pedropicapiedra : public Personaje
{
    Q_OBJECT
public:
    explicit Pedropicapiedra(QObject *parent = nullptr);

    void mover() override;
    void avance(int fase) override;

    void cargarFuerza(float valor);
    Roca* lanzarRoca();

    void gritoGuerra();
    void activarPunteria();
    void desactivarPunteria();
    void actualizarPunteria(bool usarGravedad = false);

    bool  pusteriaActiva;
    bool  superRocaLista;
    float fuerzaCargada;
    Roca *rocaRef;
    IndicadorPunteria *indicadorPunteria;

    enum Animacion { IDLE, CELEBRAR, GRITO, LANZAR, CAMINAR };

    void reproducirAnimacion(Animacion anim, bool loop = true);

private:
    static constexpr int FPS_ANIMACION = 80;
    QVector<QPixmap> framesIdle;
    QVector<QPixmap> framesCelebrar;
    QVector<QPixmap> framesGrito;
    QVector<QPixmap> framesLanzar;
    QVector<QPixmap> framesCaminar;

    QVector<QPixmap> *framesActuales;

    Animacion animActual;
    int       frameActual;
    bool      loopActivo;
    bool      algunaCargada;

    QTimer *timerSprite;
    QTimer *timerAnimTemporal;

    void cargarRango(QVector<QPixmap> &vec, QVector<int> numeros);
    void cargarFallback();
    void mostrarFrameActual();

private slots:
    void avanzarFrame();
    void onAnimacionTerminada();
};

#endif // PEDROPICAPIEDRA_H