#ifndef INDICADORPUNTERIA_H
#define INDICADORPUNTERIA_H

#include <QGraphicsItemGroup>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QObject>
#include <QTimer>
#include <QVector>

// Indicador de puntería - Habilidad 2 de Pedro
class IndicadorPunteria : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:
    explicit IndicadorPunteria(QObject *parent = nullptr);

    void mostrar(float px, float py, float fuerza, bool usarGravedad = false);
    void ocultar();
    void actualizar(float px, float py, float fuerza, bool usarGravedad = false);

private:
    QVector<QGraphicsEllipseItem*> puntos;
    QGraphicsPolygonItem *flecha;
    bool visible;
};

#endif // INDICADORPUNTERIA_H
