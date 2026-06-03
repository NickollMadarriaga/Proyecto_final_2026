#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QLabel>
#include <QTimer>
#include "juegobolos.h"
#include "nivel.h"
#include "nivelcueva.h"
#include "nivelbarranco.h"
#include "pedropicapiedra.h"

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
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    JuegoBolos     *juego;
    Nivel          *nivelActual;
    bool            enMenu, esperando, modoParabolico;
    float           angulo;

    QLabel *lTiros,*lPuntos,*lNivel,*lGrito,*lPunteria,*lHud,*lMsg;
    QTimer *tVerif;

    void mostrarMenu();
    void iniciarJuego();
    void conectar();
    void actualizarHUD();
    void mensaje(const QString&m, int ms=2000);
    void procesarTiro();
    void siguiente();
    void gameOver(bool victoria);
    PedroPicapiedra* getPedro();
};
#endif //MAINWINDOW_H