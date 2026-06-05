#include "indicadorpunteria.h"
#include <cmath>

IndicadorPunteria::IndicadorPunteria(QGraphicsScene *s, QObject *parent)
    : QObject(parent), scene(s) {}

IndicadorPunteria::~IndicadorPunteria() { limpiar(); }

void IndicadorPunteria::limpiar() {
    for (auto *p : puntos) { scene->removeItem(p); delete p; }
    puntos.clear();
}

void IndicadorPunteria::agregarPunto(float x, float y, int idx, int total) {
    int alpha = 255 - (idx * 200 / total);
    int sz    = 14  - (idx * 6 / total);
    if (sz < 5) sz = 5;
    auto *dot = new QGraphicsEllipseItem(x-sz/2, y-sz/2, sz, sz);
    dot->setBrush(QColor(255, 220, 50, alpha));
    dot->setPen(QPen(QColor(220, 130, 0, alpha), 1));
    dot->setZValue(20);
    scene->addItem(dot);
    puntos.append(dot);
}

void IndicadorPunteria::mostrarRecto(float px, float py, float fuerza) {
    limpiar();

    float sx = px + 27;
    float sy = py + 27;
    float vx = fuerza;
    int N = 14;
    for (int i = 0; i < N; i++) {
        for (int s = 0; s < 12; s++) {
            sx += vx;
            vx *= 0.999f;
        }
        agregarPunto(sx, sy, i, N);
    }
}

void IndicadorPunteria::mostrarParabolico(float px, float py,
                                          float velocidad, float anguloGrados) {
    limpiar();

    const double g  = 9.8;
    const double dt = 0.030;
    double angRad   = anguloGrados * M_PI / 180.0;
    double vx       =  velocidad * std::cos(angRad);
    double vy       = -velocidad * std::sin(angRad);
    double ox       = px + 27.5;
    double oy       = py + 27.5;

    int N = 16;
    for (int i = 0; i < N; i++) {
        double t  = (i + 1) * 5 * dt;
        double nx = ox + vx * t;
        double ny = oy + vy * t + 0.5 * g * t * t;

        if (ny > 600 || nx > 1060 || nx < -50) break;
        agregarPunto(float(nx), float(ny), i, N);
    }
}

void IndicadorPunteria::ocultar() { limpiar(); }