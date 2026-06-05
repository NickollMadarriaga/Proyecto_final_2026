#ifndef ROCA_H
#define ROCA_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPointF>
class PedroPicapiedra;
class Roca : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Roca(QObject *parent = nullptr);

    // Nivel 1: horizontal con fricción
    void lanzar(float vx);

    // Nivel 2: parabólico
    void lanzarParabolico( PedroPicapiedra* pedro, float anguloGrados);

    void activarSuperRoca();
    void cambiarVelocidad(float v);
    void resetear(float px, float py);

    float velocidadX, velocidadY;
    bool  estaActiva, superRoca, usarGravedad;
    float sueloY;

signals:
    void pinoGolpeado();
    void rocaDetenida();
    void rocaFueraPantalla();

private slots:
    void moverHorizontal();   // nivel 1
    void moverParabolico();   // nivel 2

private:
    QTimer  *timerH;          // timer nivel 1
    QTimer  *timerP;          // timer nivel 2

    // Para movimiento parabólico
    double   tiempo;
    double   velLanzamiento;
    double   anguloRad;
    QPointF  origen;

    float    friccion;
    int      tamBase;

    void detectarColisiones();
    void setImagenNormal();
};
#endif