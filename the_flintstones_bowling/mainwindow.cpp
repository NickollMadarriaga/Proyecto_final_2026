#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nivelcueva.h"
#include "nivelbarranco.h"
#include <QTimer>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    scene->setSceneRect(0,0,1000,600);
    nivelActual = new NivelCueva(scene);

    nivelActual->cargarNivel();
    lineaPunteria = scene->addLine(100,500,300,450);
    lineaPunteria->hide();

}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        nivelActual->getRoca()->lanzar(fuerza);
        QTimer::singleShot(4000, this, [=]()
         {
          if(nivelActual->nivelCompletado())
         {
          delete nivelActual;

        nivelActual = new NivelBarranco(scene);

        nivelActual->cargarNivel();
                               }
                           });
    }

    if(event->key() == Qt::Key_Up)
    {
        fuerza += 1;
    }

    if(event->key() == Qt::Key_Down)
    {
        fuerza -= 1;

        if(fuerza < 1)
        {
            fuerza = 1;
        }
    }

    if(event->key() == Qt::Key_G)
    {
        nivelActual->getRoca()->activarSuperRoca();
    }

    if(event->key() == Qt::Key_P)
    {
        lineaPunteria->setVisible(
            !lineaPunteria->isVisible()
            );
    }
}
MainWindow::~MainWindow()
{
    delete ui;
}
