#include "juegobolos.h"

JuegoBolos::JuegoBolos(QObject *parent) : QObject(parent)
{
    puntuacion     = 0;
    nivelActual    = 1;
    tirosRestantes = 5;
}

void JuegoBolos::iniciar()
{
    puntuacion     = 0;
    nivelActual    = 1;
    tirosRestantes = 5;
}

bool JuegoBolos::verificarFin()
{
    return tirosRestantes <= 0;
}

void JuegoBolos::agregarPuntos(int pts)
{
    puntuacion += pts;
    emit puntuacionActualizada(puntuacion);
}

void JuegoBolos::siguienteNivel()
{
    nivelActual++;
    tirosRestantes = 5;
    emit nivelCambiado(nivelActual);
}

void JuegoBolos::reiniciar()
{
    iniciar();
}
