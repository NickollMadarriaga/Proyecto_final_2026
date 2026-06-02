#include "Pedropicapiedra.h"

Pedropicapiedra::Pedropicapiedra(QObject *parent)
    : Personaje(parent)
{
    fuerzaCargada   = 5.0f;
    pusteriaActiva  = false;
    superRocaLista  = false;
    rocaRef         = nullptr;
    animActual      = IDLE;
    frameActual     = 0;
    loopActivo      = true;
    algunaCargada   = false;
    framesActuales  = &framesIdle;

    indicadorPunteria = new IndicadorPunteria(this);

    cargarRango(framesIdle, {1,2,3,4,5});

    cargarRango(framesCelebrar, {6,7,8,9});

    QVector<int> numerosGrito;
    for (int i = 29; i <= 51; i++) numerosGrito << i;
    numerosGrito << 100 << 101 << 102 << 103;
    cargarRango(framesGrito, numerosGrito);

    QVector<int> numerosLanzar;
    for (int i = 52; i <= 83; i++) numerosLanzar << i;
    cargarRango(framesLanzar, numerosLanzar);


    QVector<int> numerosCaminar;
    for (int i = 84; i <= 92; i++) numerosCaminar << i;
    cargarRango(framesCaminar, numerosCaminar);

    if (!algunaCargada) {
        cargarFallback();
    }

    timerSprite = new QTimer(this);
    timerSprite->setInterval(FPS_ANIMACION);
    connect(timerSprite, &QTimer::timeout, this, &Pedropicapiedra::avanzarFrame);

    timerAnimTemporal = new QTimer(this);
    timerAnimTemporal->setSingleShot(true);
    connect(timerAnimTemporal, &QTimer::timeout,
            this, &Pedropicapiedra::onAnimacionTerminada);

    reproducirAnimacion(IDLE, true);
}

void Pedropicapiedra::cargarRango(QVector<QPixmap> &vec, QVector<int> numeros)
{
    for (int n : numeros) {
        QPixmap px(QString(":/imagenes/fred/%1.png").arg(n));
        if (!px.isNull()) {
            vec << px.scaled(201, 151, Qt::KeepAspectRatio,
                             Qt::SmoothTransformation);
            algunaCargada = true;
        }
    }

    if (vec.isEmpty()) {
        QPixmap fb(120, 120);
        fb.fill(Qt::transparent);
        vec << fb;
    }
}

void Pedropicapiedra::cargarFallback()
{
    QPixmap img(":/imagenes/fred.png");
    if (!img.isNull())
        setPixmap(img.scaled(201, 151, Qt::KeepAspectRatio));
    else {
        QPixmap fb(120, 120);
        fb.fill(Qt::blue);
        setPixmap(fb);
    }
}

void Pedropicapiedra::mostrarFrameActual()
{
    if (!framesActuales || framesActuales->isEmpty()) return;

    frameActual = qBound(0, frameActual, framesActuales->size() - 1);
    setPixmap(framesActuales->at(frameActual));
}

void Pedropicapiedra::reproducirAnimacion(Animacion anim, bool loop)
{
    animActual  = anim;
    frameActual = 0;
    loopActivo  = loop;

    switch (anim) {
    case IDLE:     framesActuales = &framesIdle;     break;
    case CELEBRAR: framesActuales = &framesCelebrar; break;
    case GRITO:    framesActuales = &framesGrito;    break;
    case LANZAR:   framesActuales = &framesLanzar;   break;
    case CAMINAR:  framesActuales = &framesCaminar;  break;
    }

    mostrarFrameActual();
    timerSprite->start(FPS_ANIMACION);

    if (!loop) {
        int duracion = framesActuales->size() * FPS_ANIMACION;
        timerAnimTemporal->start(duracion);
    } else {
        timerAnimTemporal->stop();
    }
}

void Pedropicapiedra::avanzarFrame()
{
    if (!framesActuales || framesActuales->isEmpty()) return;

    frameActual++;

    if (frameActual >= framesActuales->size()) {
        if (loopActivo) {
            frameActual = 0;
        } else {
            frameActual = framesActuales->size() - 1;
            timerSprite->stop();
            return;
        }
    }

    mostrarFrameActual();
}

void Pedropicapiedra::onAnimacionTerminada()
{
    reproducirAnimacion(IDLE, true);
}

void Pedropicapiedra::gritoGuerra()
{
    superRocaLista = true;
    if (rocaRef) rocaRef->activarSuperRoca();
    reproducirAnimacion(GRITO, false);
}

Roca* Pedropicapiedra::lanzarRoca()
{
    reproducirAnimacion(LANZAR, false);
    return rocaRef;
}

void Pedropicapiedra::avance(int fase)
{
    if (fase == 1) reproducirAnimacion(CELEBRAR, false);
}

void Pedropicapiedra::mover() {}

void Pedropicapiedra::cargarFuerza(float valor)
{
    fuerzaCargada = qBound(2.0f, fuerzaCargada + valor, 20.0f);
}

void Pedropicapiedra::activarPunteria()
{
    pusteriaActiva = true;
    if (rocaRef && scene()) {
        if (!indicadorPunteria->scene())
            scene()->addItem(indicadorPunteria);
        indicadorPunteria->mostrar(rocaRef->x(), rocaRef->y(), fuerzaCargada);
    }
}

void Pedropicapiedra::desactivarPunteria()
{
    pusteriaActiva = false;
    indicadorPunteria->ocultar();
}

void Pedropicapiedra::actualizarPunteria(bool usarGravedad)
{
    if (pusteriaActiva && rocaRef)
        indicadorPunteria->actualizar(rocaRef->x(), rocaRef->y(),
                                      fuerzaCargada, usarGravedad);
}