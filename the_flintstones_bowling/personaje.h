#ifndef PERSONAJE_H
#define PERSONAJE_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Personaje : public QObject,
                  public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    explicit Personaje();

protected:
    float velocidadX;
    float velocidadY;
};

#endif // PERSONAJE_H