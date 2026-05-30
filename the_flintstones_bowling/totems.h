#ifndef TOTEMS_H
#define TOTEMS_H
#include <QGraphicsPixmapItem>
#include <QObject>
#include <QTimer>
#include <QGraphicsScene>

class Totems : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Totems(QObject *parent = nullptr);

    void derribar();
    bool estaDerrribado();

    void animarCaida();

    bool derribado;
    float anguloRotacion;

private slots:
    void actualizarAnimacion();

private:
    QTimer *timerAnimacion;
    float velocidadRotacion;
    float targetRotacion;
};

#endif // TOTEMS_H