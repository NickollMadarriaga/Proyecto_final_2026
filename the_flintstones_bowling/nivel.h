#ifndef NIVEL_H
#define NIVEL_H
#include <QGraphicsScene>
#include <QVector>
#include <QString>
#include "roca.h"
#include "totems.h"

class Nivel {
public:
    explicit Nivel(QGraphicsScene *scene);
    virtual ~Nivel();
    virtual void cargarNivel()      = 0;
    virtual void configurarFisica() = 0;
    virtual void actualizarFisica() = 0;
    virtual void avance(int fase)   = 0;
    int getTotemsRestantes();

    Roca* getRoca();
    bool  nivelCompletado();
    void  resetearRoca();

    int tirosRestantes;
    int totemsRestantes;

protected:
    QGraphicsScene   *scene;
    Roca             *roca;
    QVector<Totems*>  listaTotems;
    float rocaIniX, rocaIniY;
};
#endif //NIVEL_H