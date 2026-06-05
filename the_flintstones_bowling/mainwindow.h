#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QLabel>
#include <QTimer>
#include <QRectF>
#include<Qdebug>
#include <QGraphicsItem>
#include "juegobolos.h"
#include "nivel.h"
#include "nivelcueva.h"
#include "nivelbarranco.h"
#include "pedropicapiedra.h"
#include "indicadorpunteria.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent=nullptr);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *e) override;
private slots:
    void onPinoGolpeado();
    void onRocaDetenida();
    void onVerificacion();
    void lanzarRocaReal();
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    float x,y,ancho,alto;
    JuegoBolos     *juego;
    Nivel          *nivelActual;
    IndicadorPunteria *indicador;
    bool  enMenu, esperando, modoParabolico, pusteriaVisible;
    float angulo;  // ángulo nivel 2 en grados (positivo = hacia arriba)
    bool tiroPendiente;
    QLabel *lTiros,*lPuntos,*lNivel,*lGrito,*lPunteria,*lHud,*lMsg;
    QTimer *tVerif;

    void mostrarMenu();
    void iniciarJuego();
    void conectar();
    void actualizarHUD();
    void mensaje(const QString&m, int ms=2000);
    void refrescarPunteria();
    void procesarTiro();
    void siguiente();
    void gameOver(bool victoria);
    PedroPicapiedra* getPedro();
};
#endif