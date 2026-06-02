#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include "juegobolos.h"
#include "nivel.h"
#include "nivelcueva.h"
#include "nivelbarranco.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onPinoGolpeado();
    void onRocaDetenida();
    void onTiempoVerificacion();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;


    JuegoBolos     *juego;
    Nivel          *nivelActual;
    bool            enMenu;
    bool            esperandoVerificacion;


    QLabel *labelTiros;
    QLabel *labelPuntaje;
    QLabel *labelNivel;
    QLabel *labelHabilidades;
    QLabel *labelMensaje;


    QTimer *timerVerificacion;


    void mostrarMenu();
    void iniciarJuego();
    void actualizarHUD();
    void mostrarMensaje(const QString &msg, int duracionMs = 2000);
    void procesarTiro();
    void siguiente();
    void mostrarGameOver(bool victoria);
    void conectarNivel();


    float anguloLanzamiento;
    bool modoParabolico;
};

#endif // MAINWINDOW_H