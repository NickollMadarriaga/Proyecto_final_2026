#ifndef NIVELCUEVA_H
#define NIVELCUEVA_H

#include "nivel.h"
#include "pedropicapiedra.h"
#include <QGraphicsRectItem>

class NivelCueva : public Nivel
{
public:
    explicit NivelCueva(QGraphicsScene *scene);

    void cargarNivel()      override;
    void configurarFisica() override;
    void actualizarFisica() override;
    void avance(int fase)   override;

    PedroPicapiedra *Pedro;

private:
    float friccion;
    QString pistaDir;

    void crearSuelo();
    void colocarPinos();
};

#endif // NIVELCUEVA_H