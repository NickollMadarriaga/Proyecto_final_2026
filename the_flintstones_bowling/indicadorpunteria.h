#ifndef INDICADORPUNTERIA_H
#define INDICADORPUNTERIA_H
#include <QObject>
#include <QGraphicsScene>
#include <QVector>
#include <QGraphicsEllipseItem>

class IndicadorPunteria : public QObject {
    Q_OBJECT
public:
    explicit IndicadorPunteria(QGraphicsScene *scene, QObject *parent=nullptr);
    ~IndicadorPunteria();

    // Nivel 1: línea recta
    void mostrarRecto(float px, float py, float fuerza);
    // Nivel 2: arco parabólico real (misma física que Roca)
    void mostrarParabolico(float px, float py, float velocidad, float anguloGrados);
    void ocultar();

private:
    QGraphicsScene *scene;
    QVector<QGraphicsEllipseItem*> puntos;
    void limpiar();
    void agregarPunto(float x, float y, int idx, int total);
};
#endif // INDICADORPUNTERIA_H
