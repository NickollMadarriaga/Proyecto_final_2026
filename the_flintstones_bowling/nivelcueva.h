#ifndef NIVELCUEVA_H
#define NIVELCUEVA_H
#include "nivel.h"
#include "pedropicapiedra.h"

class NivelCueva : public Nivel {
public:
    explicit NivelCueva(QGraphicsScene *scene);
    void cargarNivel()      override;
    void configurarFisica() override;
    void actualizarFisica() override;
    void avance(int fase)   override;
    PedroPicapiedra *pedro;
};
#endif //NIVELCUEVA_H