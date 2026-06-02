#ifndef ROCA_H
#define ROCA_H

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Roca : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Roca(QObject *parent = nullptr);


    void lanzar(float velocidad);
    void lanzarParabolico(float vx, float vy);


    bool colisiona();


    void activarSuperRoca();
    void cambiarVelocidad(float nuevaVelocidad);

    void resetear(float px, float py);

    float velocidadX;
    float velocidadY;
    float radio;
    bool estaActiva;
    bool superRoca;
    bool usarGravedad;

signals:
    void totemGolpeado();
    void rocaDetenida();

private slots:
    void mover();

private:
    QTimer *timer;
    float gravedad;
    float friccion;
    int tamanoOriginal;
};

#endif // ROCA_H