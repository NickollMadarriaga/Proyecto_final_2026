#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include "roca.h"
#include <QKeyEvent>
#include <QGraphicsTextItem>
#include "nivel.h"
#include <QGraphicsLineItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    int puntaje;
    Nivel *nivelActual;
    QGraphicsLineItem *lineaPunteria;
    QGraphicsTextItem *textoPuntaje;

    int fuerza = 20;
};
#endif // MAINWINDOW_H
