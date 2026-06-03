#ifndef INDICADORPUNTERIA_H
#define INDICADORPUNTERIA_H
#include <QObject>
#include <QGraphicsItemGroup>
#include <QGraphicsEllipseItem>
#include <QVector>

class IndicadorPunteria : public QObject, public QGraphicsItemGroup {
    Q_OBJECT
public:
    explicit IndicadorPunteria(QObject *parent = nullptr);
    void mostrar(float px, float py, float fuerza, bool gravedad=false);
    void actualizar(float px, float py, float fuerza, bool gravedad=false);
    void ocultar();
private:
    QVector<QGraphicsEllipseItem*> puntos;
    void recalcular(float px, float py, float fuerza, bool gravedad);
};
#endif // INDICADORPUNTERIA_H
