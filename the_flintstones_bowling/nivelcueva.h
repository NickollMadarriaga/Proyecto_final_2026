#ifndef NIVELCUEVA_H
#define NIVELCUEVA_H

#include "nivel.h"

class NivelCueva : public Nivel
{
public:
    explicit NivelCueva(QGraphicsScene *scene);

    void cargarNivel() override;
};

#endif // NIVELCUEVA_H
