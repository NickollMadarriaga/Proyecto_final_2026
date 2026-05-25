#ifndef NIVEL_H
#define NIVEL_H
#include "roca.h"
#include <QGraphicsScene>
#include <QVector>
#include "totems.h"

class Nivel
{
public:
    explicit Nivel(QGraphicsScene *scene);

    virtual void cargarNivel() = 0;
    Roca* getRoca();
    int getTotemsRestantes();
    bool nivelCompletado();

protected:
    QGraphicsScene *scene;
    Roca *roca;
    int totemsRestantes;
    QVector<Totems*> listaTotems;
};

#endif // NIVEL_H
