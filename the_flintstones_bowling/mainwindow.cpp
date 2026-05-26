#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "nivelcueva.h"
#include "nivelbarranco.h"
#include <QTimer>
#include <QKeyEvent>
#include <QPushButton>
#include <QGraphicsTextItem>
#include <QFont>
#include <QMessageBox>

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
    setWindowTitle("The Flintstones Bowling - Tiros: 3");

}
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {
        nivelActual->getRoca()->lanzar(fuerza);
        tirosRestantes--;
        setWindowTitle("The Flintstones Bowling - Tiros: "+ QString::number(tirosRestantes));
        if(tirosRestantes <= 0)
        {
            QMessageBox::information(this,"Derrota","Se acabaron los tiros");

            delete nivelActual;

           mostrarMenu();

            tirosRestantes = 3;
        }
        QTimer::singleShot(3000, this, [=]()
        {
         if(nivelActual->nivelCompletado())
         {
          QMessageBox::information(this,"Victoria", "Nivel completado");

          delete nivelActual;

          nivelActual = new NivelBarranco(scene);

          nivelActual->cargarNivel();

          tirosRestantes = 3;
          }
        });

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
}
    void MainWindow::mostrarMenu()
     {
        scene->clear();

        QPixmap fondo(":/imagenes/menu.png");

        scene->setBackgroundBrush(
            fondo.scaled(1000,600));

        QGraphicsTextItem *titulo =
            scene->addText("THE FLINTSTONES BOWLING");

        titulo->setDefaultTextColor(Qt::white);

        titulo->setFont(
            QFont("Arial",28,QFont::Bold));

        titulo->setPos(180,100);

        botonJugar = new QPushButton("JUGAR");

        botonJugar->setGeometry(350,400,300,80);

        botonJugar->setStyleSheet("font-size:30px;" "background-color:orange;");

        connect(botonJugar,&QPushButton::clicked,this,[=]()
                {
                    delete botonJugar;

                    nivelActual =
                        new NivelCueva(scene);

                    nivelActual->cargarNivel();
                });

        botonJugar->show();
    }
    MainWindow::~MainWindow()
{
        delete ui;}

