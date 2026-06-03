#ifndef JUEGOBOLOS_H
#define JUEGOBOLOS_H
#include <QObject>
class JuegoBolos : public QObject {
    Q_OBJECT
public:
    explicit JuegoBolos(QObject *parent=nullptr);
    void iniciar();
    bool verificarFin() { return tirosRestantes<=0; }
    void agregarPuntos(int p);
    void siguienteNivel();
    int getPuntaje()  const { return puntuacion; }
    int getNivel()    const { return nivelNum; }
    int tirosRestantes;
    int puntuacion;
    int nivelNum;
signals:
    void puntuacionActualizada(int);
    void nivelCambiado(int);
};
#endif