#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>
#include <QFont>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFixedSize(1010, 610);
    scene->setSceneRect(0, 0, 1000, 600);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    juego    = new JuegoBolos(this);
    nivelActual = nullptr;
    enMenu   = true;
    esperandoVerificacion = false;
    anguloLanzamiento = -30.0f;
    modoParabolico = false;

    labelTiros = new QLabel("Tiros: 5", this);
    labelTiros->setStyleSheet(
        "color: white; font: bold 16px 'Arial';"
        "background: rgba(0,0,0,150); padding: 5px 10px; border-radius: 8px;");
    labelTiros->setGeometry(10, 10, 140, 35);

    labelPuntaje = new QLabel("Puntos: 0", this);
    labelPuntaje->setStyleSheet(
        "color: #FFD700; font: bold 16px 'Arial';"
        "background: rgba(0,0,0,150); padding: 5px 10px; border-radius: 8px;");
    labelPuntaje->setGeometry(160, 10, 160, 35);

    labelNivel = new QLabel("Nivel 1", this);
    labelNivel->setStyleSheet(
        "color: #FF8C00; font: bold 16px 'Arial';"
        "background: rgba(0,0,0,150); padding: 5px 10px; border-radius: 8px;");
    labelNivel->setGeometry(330, 10, 120, 35);

    labelHabilidades = new QLabel("[G] Grito  [P] Puntería  [↑↓] Fuerza", this);
    labelHabilidades->setStyleSheet(
        "color: #90EE90; font: 13px 'Arial';"
        "background: rgba(0,0,0,150); padding: 5px 10px; border-radius: 8px;");
    labelHabilidades->setGeometry(10, 570, 380, 30);

    labelMensaje = new QLabel("", this);
    labelMensaje->setStyleSheet(
        "color: white; font: bold 22px 'Arial';"
        "background: rgba(0,0,0,180); padding: 8px 20px; border-radius: 12px;");
    labelMensaje->setGeometry(250, 260, 520, 60);
    labelMensaje->setAlignment(Qt::AlignCenter);
    labelMensaje->hide();

    timerVerificacion = new QTimer(this);
    timerVerificacion->setSingleShot(true);
    connect(timerVerificacion, &QTimer::timeout, this, &MainWindow::onTiempoVerificacion);

    connect(juego, &JuegoBolos::puntuacionActualizada, this, [=](int pts){
        labelPuntaje->setText("Puntos: " + QString::number(pts));
    });
    connect(juego, &JuegoBolos::nivelCambiado, this, [=](int n){
        labelNivel->setText("Nivel " + QString::number(n));
    });

    setWindowTitle("The Flintstones Bowling");
    mostrarMenu();
}

void MainWindow::actualizarHUD()
{
    labelTiros->setText("Tiros: " + QString::number(juego->tirosRestantes));
    labelPuntaje->setText("Puntos: " + QString::number(juego->getPuntaje()));
    labelNivel->setText("Nivel " + QString::number(juego->getNivel()));

    if (juego->getNivel() == 2) {
        labelHabilidades->setText(
            "[ESPACIO] Lanzar  [↑↓] Ángulo  [G] Grito  [P] Puntería");
    } else {
        labelHabilidades->setText(
            "[ESPACIO] Lanzar  [↑↓] Fuerza  [G] Grito  [P] Puntería");
    }
}

void MainWindow::mostrarMensaje(const QString &msg, int duracionMs)
{
    labelMensaje->setText(msg);
    labelMensaje->show();
    QTimer::singleShot(duracionMs, this, [=](){ labelMensaje->hide(); });
}

void MainWindow::mostrarMenu()
{
    enMenu = true;
    scene->clear();

    QPixmap fondo(":/imagenes/menu.png");
    if (!fondo.isNull())
        scene->setBackgroundBrush(fondo.scaled(1000, 600));
    else
        scene->setBackgroundBrush(QBrush(QColor(40, 20, 10)));

    // Título
    QGraphicsTextItem *titulo = scene->addText("THE FLINTSTONES\nBOWLING");
    titulo->setDefaultTextColor(QColor(255, 200, 50));
    titulo->setFont(QFont("Arial", 34, QFont::Bold));
    titulo->setPos(240, 80);

    // Subtítulo
    QGraphicsTextItem *sub = scene->addText("Presiona ENTER para jugar");
    sub->setDefaultTextColor(Qt::white);
    sub->setFont(QFont("Arial", 18));
    sub->setPos(310, 240);

    // Instrucciones
    QGraphicsTextItem *instruc = scene->addText(
        "CONTROLES:\n"
        "  ESPACIO → Lanzar roca\n"
        "  ↑ / ↓  → Aumentar / Reducir fuerza\n"
        "  G       → Grito de guerra (super roca)\n"
        "  P       → Puntería (ver trayectoria)\n\n"
        "NIVEL 2:\n"
        "  ↑ / ↓  → Ajustar ángulo de lanzamiento");
    instruc->setDefaultTextColor(QColor(220, 220, 180));
    instruc->setFont(QFont("Arial", 14));
    instruc->setPos(260, 300);

    labelTiros->hide();
    labelPuntaje->hide();
    labelNivel->hide();
    labelHabilidades->hide();
}

void MainWindow::iniciarJuego()
{
    enMenu = false;
    juego->iniciar();

    labelTiros->show();
    labelPuntaje->show();
    labelNivel->show();
    labelHabilidades->show();

    nivelActual = new NivelCueva(scene);
    nivelActual->cargarNivel();
    modoParabolico = false;
    conectarNivel();
    actualizarHUD();
}

void MainWindow::conectarNivel()
{
    if (!nivelActual || !nivelActual->getRoca()) return;

    connect(nivelActual->getRoca(), &Roca::totemGolpeado,
            this, &MainWindow::onPinoGolpeado);
    connect(nivelActual->getRoca(), &Roca::rocaDetenida,
            this, &MainWindow::onRocaDetenida);
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (enMenu) {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
            iniciarJuego();
        return;
    }
    if (!nivelActual) return;


    Pedropicapiedra *Pedro = nullptr;
    if (auto *nc = dynamic_cast<NivelCueva*>(nivelActual))      Pedro = nc->Pedro;
    if (auto *nb = dynamic_cast<NivelBarranco*>(nivelActual))   Pedro = nb->Pedro;

    Roca *roca = nivelActual->getRoca();
    if (!roca) return;

    switch (event->key()) {


    case Qt::Key_Space:
        if (roca->estaActiva || esperandoVerificacion) break;
        procesarTiro();
        break;

    case Qt::Key_Up:
        if (Pedro) {
            if (modoParabolico) {
                anguloLanzamiento = qBound(-70.0f, anguloLanzamiento - 5.0f, -5.0f);
                mostrarMensaje("Ángulo: " + QString::number((int)-anguloLanzamiento) + "°", 600);
            } else {
                Pedro->cargarFuerza(1.0f);
                mostrarMensaje("Fuerza: " + QString::number((int)Pedro->fuerzaCargada), 600);
            }
            if (Pedro->pusteriaActiva)
                Pedro->actualizarPunteria(modoParabolico);
        }
        break;

    case Qt::Key_Down:
        if (Pedro) {
            if (modoParabolico) {
                anguloLanzamiento = qBound(-70.0f, anguloLanzamiento + 5.0f, -5.0f);
                mostrarMensaje("Ángulo: " + QString::number((int)-anguloLanzamiento) + "°", 600);
            } else {
                Pedro->cargarFuerza(-1.0f);
                mostrarMensaje("Fuerza: " + QString::number((int)Pedro->fuerzaCargada), 600);
            }
            if (Pedro->pusteriaActiva)
                Pedro->actualizarPunteria(modoParabolico);
        }
        break;

    case Qt::Key_G:
        if (Pedro && !roca->estaActiva) {
            Pedro->gritoGuerra();
            mostrarMensaje("🦴 ¡YABBA DABBA DOO! ¡Super Roca activada!", 2000);
        }
        break;

    case Qt::Key_P:
        if (Pedro) {
            if (Pedro->pusteriaActiva) {
                Pedro->desactivarPunteria();
                mostrarMensaje("Puntería desactivada", 800);
            } else {
                Pedro->activarPunteria();
                Pedro->actualizarPunteria(modoParabolico);
                mostrarMensaje("🎯 Puntería activada", 800);
            }
        }
        break;

    default:
        break;
    }
}

void MainWindow::procesarTiro()
{
    if (juego->tirosRestantes <= 0) return;

    Roca *roca = nivelActual->getRoca();
    Pedropicapiedra *Pedro = nullptr;
    if (auto *nc = dynamic_cast<NivelCueva*>(nivelActual))    Pedro = nc->Pedro;
    if (auto *nb = dynamic_cast<NivelBarranco*>(nivelActual)) Pedro = nb->Pedro;

    float fuerza = Pedro ? Pedro->fuerzaCargada : 8.0f;

    if (modoParabolico) {

        float rad = anguloLanzamiento * M_PI / 180.0f;
        float vx  = fuerza * std::cos(rad);
        float vy  = fuerza * std::sin(rad);
        roca->lanzarParabolico(vx, vy);
    } else {

        roca->lanzar(fuerza);
    }

    juego->tirosRestantes--;
    esperandoVerificacion = true;

    if (Pedro && Pedro->pusteriaActiva)
        Pedro->desactivarPunteria();

    actualizarHUD();

    timerVerificacion->start(3500);
}

void MainWindow::onPinoGolpeado()
{
    juego->agregarPuntos(100);
    actualizarHUD();
}

void MainWindow::onRocaDetenida()
{
    if (!esperandoVerificacion) return;
    if (!timerVerificacion->isActive())
        timerVerificacion->start(1500);
}

void MainWindow::onTiempoVerificacion()
{
    esperandoVerificacion = false;

    if (nivelActual->nivelCompletado()) {
        siguiente();
        return;
    }

    if (juego->tirosRestantes <= 0) {
        mostrarGameOver(false);
        return;
    }

    nivelActual->resetearRoca();
    Roca *roca = nivelActual->getRoca();
    if (roca) {
        connect(roca, &Roca::totemGolpeado, this, &MainWindow::onPinoGolpeado);
        connect(roca, &Roca::rocaDetenida, this, &MainWindow::onRocaDetenida);
    }
    mostrarMensaje("Tiro listo → ESPACIO para lanzar", 1500);
}


void MainWindow::siguiente()
{
    if (juego->getNivel() == 1) {
        mostrarMensaje("🎉 ¡Nivel 1 completado! Pasando al Barranco...", 2500);
        juego->agregarPuntos(500);
        juego->siguienteNivel();

        QTimer::singleShot(2600, this, [=]() {
            delete nivelActual;
            nivelActual = new NivelBarranco(scene);
            nivelActual->cargarNivel();
            modoParabolico = true;
            conectarNivel();
            actualizarHUD();
            mostrarMensaje("Nivel 2 - ¡Cuidado con el Dino!", 2000);
        });
    } else {
        mostrarGameOver(true);
    }
}

void MainWindow::mostrarGameOver(bool victoria)
{
    labelMensaje->hide();
    scene->clear();

    QColor fondo = victoria ? QColor(20, 80, 20) : QColor(80, 20, 20);
    scene->setBackgroundBrush(QBrush(fondo));

    QString titulo = victoria
                         ? "🏆 ¡VICTORIA! ¡Yabba Dabba Doo!"
                         : "💀 Game Over";

    QGraphicsTextItem *t = scene->addText(titulo);
    t->setDefaultTextColor(victoria ? QColor(255, 220, 50) : Qt::red);
    t->setFont(QFont("Arial", 28, QFont::Bold));
    t->setPos(150, 150);

    QGraphicsTextItem *pts = scene->addText(
        "Puntuación final: " + QString::number(juego->getPuntaje()) + " pts");
    pts->setDefaultTextColor(Qt::white);
    pts->setFont(QFont("Arial", 20));
    pts->setPos(280, 250);

    QGraphicsTextItem *volver = scene->addText("Presiona ENTER para volver al menú");
    volver->setDefaultTextColor(QColor(200, 200, 200));
    volver->setFont(QFont("Arial", 16));
    volver->setPos(230, 340);

    delete nivelActual;
    nivelActual = nullptr;
    enMenu = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}