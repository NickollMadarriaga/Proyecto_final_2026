#ifndef NIVELBARRANCO_H
#define NIVELBARRANCO_H

#include "nivel.h"

class NivelBarranco : public Nivel
{
public:
    explicit NivelBarranco(QGraphicsScene *scene);

    void cargarNivel() override;
};

#endif // NIVELBARRANCO_H