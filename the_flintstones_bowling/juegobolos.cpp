#include "juegobolos.h"
JuegoBolos::JuegoBolos(QObject *parent):QObject(parent),tirosRestantes(5),puntuacion(0),nivelNum(1){}
void JuegoBolos::iniciar(){puntuacion=0;nivelNum=1;tirosRestantes=5;}
void JuegoBolos::agregarPuntos(int p){puntuacion+=p;emit puntuacionActualizada(puntuacion);}
void JuegoBolos::siguienteNivel(){nivelNum++;tirosRestantes=5;emit nivelCambiado(nivelNum);}