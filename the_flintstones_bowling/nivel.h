#ifndef NIVEL_H
#define NIVEL_H
#include <QGraphicsScene>
#include <QVector>
#include <QString>
#include "roca.h"
#include "totems.h"
#include "Dino.h"

class Nivel {
public:
    explicit Nivel(QGraphicsScene *scene);
    virtual ~Nivel();
    virtual void cargarNivel()      = 0;
    virtual void configurarFisica() = 0;
    virtual void actualizarFisica() = 0;
    virtual void avance(int fase)   = 0;
    int getTotemsRestantes();
    virtual Dino* getDino() { return nullptr; }
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
    Dino *dino;
};
#endif //NIVEL_H