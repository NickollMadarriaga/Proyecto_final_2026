#ifndef NIVEL_H
#define NIVEL_H

#include <QGraphicsScene>
#include <QVector>
#include <QString>
#include "roca.h"
#include "totems.h"

class Nivel
{
public:
    explicit Nivel(QGraphicsScene *scene);
    virtual ~Nivel();

    virtual void cargarNivel()          = 0;
    virtual void configurarFisica()     = 0;
    virtual void actualizarFisica()     = 0;
    virtual void avance(int fase)       = 0;

    Roca* getRoca();
    int   getTotemsRestantes();
    bool  nivelCompletado();
    void  resetearRoca();

protected:
    QGraphicsScene   *scene;
    Roca             *roca;
    QVector<Totems*>    listaTotems;
    int               tirosRestantes;
    int               totemsRestantes;
    QString           nombre;
};

#endif // NIVEL_H
