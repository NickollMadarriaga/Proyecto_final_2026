#ifndef NIVELBARRANCO_H
#define NIVELBARRANCO_H
#include "nivel.h"
#include "pedropicapiedra.h"
#include "dino.h"
#include <QVector>

class NivelBarranco : public Nivel {
public:
    explicit NivelBarranco(QGraphicsScene *scene);
    void cargarNivel()      override;
    void configurarFisica() override;
    void actualizarFisica() override;
    void avance(int fase)   override;

    PedroPicapiedra *fred;
    Dino            *dino;

private:
    QVector<QRectF> plats;
    void mkPlataformas();
    void mkTotems();
};
#endif