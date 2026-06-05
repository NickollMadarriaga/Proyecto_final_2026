#ifndef PEDROPICAPIEDRA_H
#define PEDROPICAPIEDRA_H
#include "personaje.h"
#include "roca.h"
#include <QVector>
#include <QTimer>
#include <QMediaPlayer>
#include <QAudioOutput>


class PedroPicapiedra : public Personaje {
    Q_OBJECT
public:
    explicit PedroPicapiedra(QObject *parent = nullptr);
    void mover()          override;
    void avance(int fase) override;

    void cargarFuerza(float v);
    void lanzarRoca();

    // Habilidad 1 — 1 uso por nivel
    void gritoGuerra();
    bool gritoDisponible;

    // Habilidad 2 — solo el flag, la puntería la maneja MainWindow
    bool pusteriaDisponible;

    float fuerzaCargada;
    Roca *rocaRef;

    enum Anim { IDLE, CELEBRAR, GRITO, LANZAR, CAMINAR };
    void play(Anim a, bool loop=true);
signals:
    void soltarRoca();
private:
    static const int FPS = 30;
    QVector<QPixmap> fIdle, fCelebrar, fGrito, fLanzar, fCaminar;
    QVector<QPixmap> *cur;
    Anim   animActual;
    int    frame;
    bool   looping;
    bool   cargado;
    QTimer *tSprite, *tTemp;
    QAudioOutput *audioMusica;
    QAudioOutput *audioFX;

    QMediaPlayer *sndGrito;

    void loadRange(QVector<QPixmap>&, QVector<int>);
    void fallback();
    void showFrame();

private slots:
    void nextFrame();
    void backIdle();
};
#endif