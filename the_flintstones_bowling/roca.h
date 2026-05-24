#ifndef ROCA_H
#define ROCA_H

#include <QObject>
#include <QGraphicsEllipseItem>
#include <QTimer>

class Roca : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    explicit Roca();

    void lanzar(float velocidad);

public slots:
    void mover();

private:
    float velocidadX;

    QTimer *timer;
};

#endif // ROCA_H
