#ifndef TOTEMS_H
#define TOTEMS_H
#include <QObject>
#include <QGraphicsPixmapItem>
#include <QTimer>

class Totems : public QObject, public QGraphicsPixmapItem {
    Q_OBJECT
public:
    explicit Totems(QObject *parent = nullptr);
    void derribar();
    bool estaDerrribado() const { return derribado; }
    bool derribado;
private slots:
    void tickAnimacion();
private:
    QTimer *timerAnim;
    float   angulo;
};

#endif // TOTEMS_H