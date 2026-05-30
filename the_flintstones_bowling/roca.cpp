#include "roca.h"
#include "totems.h"
#include <QPixmap>
#include <QList>
#include <QGraphicsItem>
#include <cmath>

Roca::Roca(QObject *parent) : QObject(parent), QGraphicsPixmapItem()
{
    QPixmap rocaImagen(":/imagenes/roca.png");
    if (rocaImagen.isNull()) {
        QPixmap fallback(60, 60);
        fallback.fill(Qt::darkGray);
        setPixmap(fallback);
    } else {
        setPixmap(rocaImagen.scaled(60, 60));
    }

    velocidadX   = 0;
    velocidadY   = 0;
    radio        = 30;
    estaActiva   = false;
    superRoca    = false;
    usarGravedad = false;
    gravedad     = 0.4f;
    friccion     = 0.992f;   // Fricción suelo de tierra (Nivel 1)
    tamanoOriginal = 60;

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Roca::mover);
    timer->start(16);
}

void Roca::lanzar(float velocidad)
{
    velocidadX = velocidad;
    velocidadY = 0;
    usarGravedad = false;
    estaActiva = true;
}

void Roca::lanzarParabolico(float vx, float vy)
{
    velocidadX = vx;
    velocidadY = vy;
    usarGravedad = true;
    estaActiva = true;
}

bool Roca::colisiona()
{
    QList<QGraphicsItem*> colisiones = collidingItems();
    bool golpeo = false;

    for (QGraphicsItem *item : colisiones) {
        Totems *totems = dynamic_cast<Totems*>(item);
        if (totems && !totems->derribado) {
            totems->derribar();
            golpeo = true;

            // Efecto de rebote lateral en super roca
            if (superRoca) {
                // La super roca sigue sin frenarse mucho
                velocidadX *= 0.95f;
            } else {
                velocidadX *= 0.75f;
            }
        }
    }
    if (golpeo) emit totemGolpeado();
    return golpeo;
}

void Roca::mover()
{
    if (!estaActiva) return;

    // Aplicar gravedad si es lanzamiento parabólico (Nivel 2)
    if (usarGravedad) {
        velocidadY += gravedad;
    }

    setPos(x() + velocidadX, y() + velocidadY);

    // Aplicar fricción del suelo (Nivel 1: tierra)
    if (!usarGravedad) {
        velocidadX *= friccion;
        if (std::abs(velocidadX) < 0.1f) {
            velocidadX = 0;
            estaActiva = false;
            emit rocaDetenida();
        }
    }

    // Límite inferior (suelo)
    if (scene() && y() > scene()->height() - 60) {
        setY(scene()->height() - 60);
        velocidadY *= -0.4f;  // pequeño rebote
        if (std::abs(velocidadY) < 0.5f) velocidadY = 0;
        // Aplicar fricción al aterrizar
        velocidadX *= friccion;
        if (std::abs(velocidadX) < 0.1f) {
            velocidadX = 0;
            estaActiva = false;
            emit rocaDetenida();
        }
    }

    // Límite derecho: salió de escena
    if (scene() && x() > scene()->width()) {
        estaActiva = false;
        emit rocaDetenida();
    }

    // Desactivar super roca cuando se frena
    if (superRoca && std::abs(velocidadX) < 0.5f) {
        superRoca = false;
        QPixmap rocaImagen(":/imagenes/roca.png");
        if (!rocaImagen.isNull())
            setPixmap(rocaImagen.scaled(tamanoOriginal, tamanoOriginal));
    }

    // Detectar colisiones
    colisiona();
}

void Roca::cambiarVelocidad(float nuevaVelocidad)
{
    velocidadX = nuevaVelocidad;
}

void Roca::activarSuperRoca()
{
    superRoca = true;
    friccion = 0.999f; // Menos fricción en super roca
    QPixmap rocaImagen(":/imagenes/roca.png");
    if (!rocaImagen.isNull())
        setPixmap(rocaImagen.scaled(100, 100));
    else {
        QPixmap fallback(100, 100);
        fallback.fill(Qt::red);
        setPixmap(fallback);
    }
}

void Roca::resetear(float px, float py)
{
    velocidadX = 0;
    velocidadY = 0;
    estaActiva = false;
    superRoca  = false;
    usarGravedad = false;
    friccion   = 0.992f;
    setPos(px, py);

    QPixmap rocaImagen(":/imagenes/roca.png");
    if (!rocaImagen.isNull())
        setPixmap(rocaImagen.scaled(tamanoOriginal, tamanoOriginal));
}