#ifndef NIVELBARRANCO_H
#define NIVELBARRANCO_H

#include "nivel.h"
#include "pedropicapiedra.h"
#include "dino.h"
#include <QGraphicsRectItem>

class NivelBarranco : public Nivel
{
public:
    explicit NivelBarranco(QGraphicsScene *scene);

    void cargarNivel()       override;
    void configurarFisica()  override;
    void actualizarFisica()  override;
    void avance(int fase)    override;


    Pedropicapiedra *Pedro;
    Dino *dino;

private:
    float gravedad;
    QVector<QGraphicsRectItem*> plataformas;

    void crearPlataformas();
    void colocarTotems();
    bool colisionConPlataforma();
};

#endif // NIVELBARRANCO_H