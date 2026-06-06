#include "indicadorpunteria.h"
#include <cmath>

IndicadorPunteria::IndicadorPunteria(QGraphicsScene *s, QObject *parent)
    : QObject(parent), scene(s) {}

IndicadorPunteria::~IndicadorPunteria() { limpiar(); }

void IndicadorPunteria::limpiar() {
    for (auto *p : puntos) {
        if (scene) scene->removeItem(p);
        delete p;
    }
    puntos.clear();
}

void IndicadorPunteria::agregarPunto(float x, float y, int idx, int total) {
    int alpha = 255 - (idx * 180 / total);
    int sz    = 13  - (idx * 5  / total);
    if (sz < 5) sz = 5;
    auto *dot = new QGraphicsEllipseItem(x - sz/2.0f, y - sz/2.0f, sz, sz);
    dot->setBrush(QColor(255, 220, 50, alpha));
    dot->setPen(QPen(QColor(220, 130, 0, alpha), 1));
    dot->setZValue(20);
    scene->addItem(dot);
    puntos.append(dot);
}

// ── NIVEL 1: rectilíneo con fricción ─────────────────────────
void IndicadorPunteria::mostrarRecto(float rocaX, float rocaY, float fuerza) {
    limpiar();
    float sx = rocaX + 55.0f;
    float sy = rocaY + 27.5f;
    float vx = fuerza;
    const float FRICCION = 0.9998f;
    for (int i = 0; i < 20; i++) {
        for (int s = 0; s < 20; s++) {
            sx += vx;
            vx *= FRICCION;
            if (vx < 0.1f) break;
        }
        if (sx > 1050.0f || vx < 0.1f) break;
        agregarPunto(sx, sy, i, 20);
    }
}

// ── NIVEL 2: parabólico ───────────────────────────────────────
// Misma física EXACTA que Roca::lanzarParabolico / moverParabolico:
//   velLanzamiento = fuerza * 40.0
//   g = 9.8
//   dt = 0.030
//   vx =  velLanzamiento * cos(angulo)
//   vy = -velLanzamiento * sin(angulo)   ← negativo = sube en Qt
//   dx = vx * t
//   dy = vy * t + 0.5 * g * t * t

void IndicadorPunteria::mostrarParabolico(float rocaX, float rocaY,
                                          float fuerza, float anguloGrados) {
    limpiar();

    // EXACTAMENTE iguales a Roca::lanzarParabolico y moverParabolico
    const double G  = 9.8;
    const double DT = 0.030;
    double velLanzamiento = fuerza * 40.0;

    double angRad = anguloGrados * M_PI / 180.0;
    double vx     =  velLanzamiento * std::cos(angRad);
    double vy     = -velLanzamiento * std::sin(angRad); // negativo = sube

    // Origen: misma posición exacta que usa la roca (x(), y())
    double ox = rocaX;
    double oy = rocaY;

    // Simular la trayectoria punto a punto
    // Cada punto = 8 ticks adelante (8 * 0.030 = 0.24s entre puntos)
    for (int i = 0; i < 20; i++) {
        double t  = (i + 1) * 8.0 * DT;
        double nx = ox + vx * t;
        double ny = oy + vy * t + 0.5 * G * t * t;

        if (ny > 620.0 || nx > 1060.0 || nx < -50.0) break;
        agregarPunto(float(nx), float(ny), i, 20);
    }
}

void IndicadorPunteria::ocultar() { limpiar(); }