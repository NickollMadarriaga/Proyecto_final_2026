#ifndef ROCA_H
#define ROCA_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Roca : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Roca(QObject *parent = nullptr);
    void lanzar(float vx);
    void lanzarParabolico(float vx, float vy);
    void activarSuperRoca();
    void cambiarVelocidad(float v);
    void resetear(float px, float py);

    float velocidadX, velocidadY;
    bool  estaActiva, superRoca, usarGravedad;
    float sueloY;   // fijado por el nivel

signals:
    void pinoGolpeado();
    void rocaDetenida();

private slots:
    void mover();
private:
    QTimer *timer;
    float   gravedad, friccion;
    int     tamBase;
    void    detectarColisiones();
};
#endif //ROCA_H