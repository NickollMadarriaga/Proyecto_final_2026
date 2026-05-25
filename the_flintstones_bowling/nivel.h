#ifndef NIVEL_H
#define NIVEL_H
#include "roca.h"
#include <QGraphicsScene>

class Nivel
{
public:
    explicit Nivel(QGraphicsScene *scene);

    virtual void cargarNivel() = 0;
    Roca* getRoca();

protected:
    QGraphicsScene *scene;
    Roca *roca;
};

#endif // NIVEL_H
