#ifndef PERSONAJE_H
#define PERSONAJE_H
#include <QObject>
#include <QGraphicsPixmapItem>

class Personaje : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Personaje(QObject *parent = nullptr);
    virtual void mover() = 0;
    virtual void avance(int fase) = 0;
protected:
    float velocidad;
};
#endif //PERSONAJE_H