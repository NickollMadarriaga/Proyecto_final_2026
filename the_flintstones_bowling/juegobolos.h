#ifndef JUEGOBOLOS_H
#define JUEGOBOLOS_H

#include <QObject>
#include "nivel.h"

class JuegoBolos : public QObject
{
    Q_OBJECT
public:
    explicit JuegoBolos(QObject *parent = nullptr);

    void iniciar();
    bool verificarFin();

    int getPuntaje()    const { return puntuacion; }
    int getNivel()      const { return nivelActual; }
    int getTirosRestantes() const { return tirosRestantes; }

    void agregarPuntos(int pts);
    void siguienteNivel();
    void reiniciar();

    int tirosRestantes;
    int puntuacion;
    int nivelActual;

signals:
    void juegoTerminado(int puntuacion);
    void nivelCambiado(int nuevoNivel);
    void puntuacionActualizada(int puntos);
};

#endif // JUEGOBOLOS_H