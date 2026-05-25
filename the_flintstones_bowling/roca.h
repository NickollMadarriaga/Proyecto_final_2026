#ifndef ROCA_H
#define ROCA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPixmap>

class Roca : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit Roca();

    void lanzar(float velocidad);
    void cambiarVelocidad(float nuevaVelocidad);
    void lanzarParabolico(float vx, float vy);
    void activarSuperRoca();

public slots:
    void mover();

private:
    bool superRoca;
    float velocidadX;
    float velocidadY;

    float gravedad;

    bool usarGravedad;

    QTimer *timer;
};

#endif // ROCA_H
