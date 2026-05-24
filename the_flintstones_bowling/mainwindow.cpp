#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "roca.h"
#include <QKeyEvent>
#include "totems.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);

    ui->graphicsView->setScene(scene);

    scene->setSceneRect(0,0,1000,600);

    scene->addRect(0,500,1000,100);
    roca = new Roca();

    roca->setPos(50,460);

    scene->addItem(roca);
    fuerza=20;
    Totems *totems1 = new Totems();
    Totems *totems2 = new Totems();
    Totems *totems3 = new Totems();

    totems1->setPos(700,420);
    totems2->setPos(750,420);
    totems3->setPos(725,340);

    scene->addItem(totems1);
    scene->addItem(totems2);
    scene->addItem(totems3);

}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        roca->lanzar(fuerza);
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
}
MainWindow::~MainWindow()
{
    delete ui;
}
