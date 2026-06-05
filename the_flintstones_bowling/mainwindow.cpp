#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QFont>
#include <QGraphicsTextItem>
#include <QLinearGradient>
#include <QPainter>
#include <cmath>

static QLabel* mkHUD(QWidget *p,QString t,QString c,int x,int y,int w,int h){
    QLabel *l=new QLabel(t,p);
    l->setStyleSheet(QString(
                         "color:%1;font:bold 13px Arial;"
                         "background:rgba(0,0,0,175);"
                         "padding:3px 9px;border-radius:7px;"
                         "border:1px solid rgba(255,190,60,110);").arg(c));
    l->setGeometry(x,y,w,h); l->setAlignment(Qt::AlignCenter);
    return l;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene=new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setFixedSize(1000,600);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);
    scene->setSceneRect(0,0,1000,600);
    setFixedSize(1000,625);
    tiroPendiente = false;
    juego=new JuegoBolos(this);
    nivelActual=nullptr; indicador=nullptr;
    enMenu=true; esperando=false; modoParabolico=false;
    audioMusica = new QAudioOutput(this);
    audioFX     = new QAudioOutput(this);

    audioMusica->setVolume(0.25);
    audioFX->setVolume(0.80);
    musicaFondo = new QMediaPlayer(this);
    musicaFondo->setAudioOutput(audioMusica);

    musicaFondo->setSource(
        QUrl("qrc:/sonidos/fondo.mp3"));
    sndLanzamiento = new QMediaPlayer(this);
    sndLanzamiento->setAudioOutput(audioFX);
    sndLanzamiento->setSource(
        QUrl("qrc:/sonidos/lanzamiento.mp3"));


    sndVictoria = new QMediaPlayer(this);
    sndVictoria->setAudioOutput(audioFX);
    sndVictoria->setSource(
        QUrl("qrc:/sonidos/victoria.mp3"));

    sndDerrota = new QMediaPlayer(this);
    sndDerrota->setAudioOutput(audioFX);
    sndDerrota->setSource(
        QUrl("qrc:/sonidos/derrota.mp3"));



    angulo=45.0f; // ángulo inicial nivel 2 (45° = tiro óptimo)

    lTiros    = mkHUD(this,"🪨 Tiros: 5",  "white",   8,  8,125,30);
    lPuntos   = mkHUD(this,"⭐ Pts: 0",   "#FFD700",143,  8,120,30);
    lNivel    = mkHUD(this,"📍 Nivel 1",  "#FF8C00",273,  8,105,30);
    lGrito    = mkHUD(this,"🦴 Grito ✅", "#90EE90",388,  8,120,30);
    lPunteria = mkHUD(this,"🎯 Puntería ✅","#87CEEB",518, 8,140,30);

    lHud=new QLabel(this);
    lHud->setStyleSheet("color:#bbb;font:11px Arial;"
                        "background:rgba(0,0,0,155);padding:3px 8px;border-radius:5px;");
    lHud->setGeometry(8,562,500,24);

    lMsg=new QLabel(this);
    lMsg->setStyleSheet("color:white;font:bold 17px Arial;"
                        "background:rgba(0,0,0,205);padding:9px 22px;border-radius:13px;"
                        "border:2px solid rgba(255,195,60,210);");
    lMsg->setGeometry(130,250,740,54);
    lMsg->setAlignment(Qt::AlignCenter); lMsg->hide();
    musicaFondo->play();
    tVerif=new QTimer(this); tVerif->setSingleShot(true);
    connect(tVerif,&QTimer::timeout,this,&MainWindow::onVerificacion);

    connect(juego,&JuegoBolos::puntuacionActualizada,this,
            [=](int p){lPuntos->setText("⭐ Pts: "+QString::number(p));});
    connect(juego,&JuegoBolos::nivelCambiado,this,
            [=](int n){lNivel->setText("📍 Nivel "+QString::number(n));});

    setWindowTitle("The Flintstones Bowling");
    mostrarMenu();
}


void MainWindow::mostrarMenu() {
    enMenu=true; scene->clear();
    if (indicador) { delete indicador; indicador=nullptr; }

    QPixmap fm(":/imagenes/menu.png");
    if (!fm.isNull())
        scene->setBackgroundBrush(fm.scaled(1000,600,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    else {
        QLinearGradient g(0,0,0,600);
        g.setColorAt(0,QColor(28,14,4)); g.setColorAt(1,QColor(75,42,12));
        scene->setBackgroundBrush(g);
    }

    scene->addRect(215,55,570,500,
                   QPen(QColor(200,158,48,230),3),QBrush(QColor(8,4,1,218)))->setZValue(1);
    scene->addLine(245,100,755,100,QPen(QColor(200,158,48,140),1))->setZValue(2);
    scene->addLine(245,500,755,500,QPen(QColor(200,158,48,140),1))->setZValue(2);

    auto txt=[&](QString t,QColor c,QFont f,float x,float y,int z=3){
        auto*i=scene->addText(t); i->setDefaultTextColor(c);
        i->setFont(f); i->setPos(x,y); i->setZValue(z); return i;
    };
    txt("THE FLINTSTONES",QColor(0,0,0,90),QFont("Impact",34,QFont::Bold),262,112,2);
    txt("THE FLINTSTONES",QColor(255,200,48),QFont("Impact",34,QFont::Bold),259,109,3);
    txt("BOWLING",QColor(0,0,0,90),QFont("Impact",52,QFont::Bold),332,152,2);
    txt("BOWLING",QColor(255,108,8),QFont("Impact",52,QFont::Bold),329,149,3);
    txt("¡Yabba Dabba Doo!",QColor(185,215,255,215),QFont("Arial",15),345,218,3);
    scene->addLine(255,250,745,250,QPen(QColor(200,158,48,120),1))->setZValue(3);

    struct Row{QString k,d;};
    QVector<Row> rows={
                         {"[ ESPACIO ]",  "Lanzar la roca"},
                         {"[ W ]  [ S ]","Fuerza / Ángulo de lanzamiento"},
                         {"[   G   ]",    "Grito de Guerra — Super Roca  (1 uso)"},
                         {"[   P   ]",    "Puntería — Ver trayectoria  (1 uso)"},
                         {"[ ENTER ]",    "Iniciar el juego"},
                         };
    int cy=262;
    for(auto&r:rows){
        txt(r.k,QColor(255,208,55),QFont("Courier New",11,QFont::Bold),238,cy,3);
        txt(r.d,QColor(212,212,192),QFont("Arial",11),432,cy,3);
        cy+=38;
    }
    scene->addLine(255,458,745,458,QPen(QColor(200,158,48,120),1))->setZValue(3);
    scene->addRect(320,465,360,52,
                   QPen(QColor(255,195,48,225),2),QBrush(QColor(155,65,5,225)))->setZValue(3);
    txt("▶   PRESIONA  ENTER  PARA  JUGAR",
        QColor(255,232,165),QFont("Arial",13,QFont::Bold),332,478,4);
    txt("v1.0  ·  2 Niveles  ·  Agente IA Activo",
        QColor(115,95,65),QFont("Arial",10),320,528,3);

    for(QLabel*l:{lTiros,lPuntos,lNivel,lGrito,lPunteria,lHud}) l->hide();
}


void MainWindow::iniciarJuego() {
    enMenu=false; juego->iniciar();
    for(QLabel*l:{lTiros,lPuntos,lNivel,lGrito,lPunteria,lHud}) l->show();

    if (indicador) { delete indicador; indicador=nullptr; }
    indicador = new IndicadorPunteria(scene, this);

    nivelActual=new NivelCueva(scene);
    nivelActual->cargarNivel();
    modoParabolico=false; angulo=45.0f;
    conectar(); actualizarHUD();
    mensaje("🪨  Nivel 1 — ¡Derriba los 5 tótems con 5 tiros!",2400);
}

void MainWindow::conectar() {
    Roca*r=nivelActual?nivelActual->getRoca():nullptr;
    if(!r) return;
    connect(r,&Roca::pinoGolpeado,this,&MainWindow::onPinoGolpeado);
    connect(r,&Roca::rocaDetenida,this,&MainWindow::onRocaDetenida);

    auto *pedro = getPedro();

    if(pedro)
    {
        connect(pedro,
                &PedroPicapiedra::soltarRoca,
                this,
                &MainWindow::lanzarRocaReal);
    }
}


PedroPicapiedra* MainWindow::getPedro() {
    if(auto*nc=dynamic_cast<NivelCueva*>(nivelActual))    return nc->pedro;
    if(auto*nb=dynamic_cast<NivelBarranco*>(nivelActual)) return nb->fred;
    return nullptr;
}


void MainWindow::actualizarHUD() {
    lTiros->setText("🪨 Tiros: "+QString::number(juego->tirosRestantes));
    lPuntos->setText("⭐ Pts: "+QString::number(juego->getPuntaje()));
    lNivel->setText("📍 Nivel "+QString::number(juego->getNivel()));
    if(auto*p=getPedro()){
        lGrito->setText(p->gritoDisponible?"🦴 Grito ✅":"🦴 Grito ❌");
        lPunteria->setText(p->pusteriaDisponible?"🎯 Puntería ✅":"🎯 Puntería ❌");
    }
    lHud->setText(modoParabolico
                      ?"[ESPACIO] Lanzar  [WS] Ángulo: "+QString::number((int)angulo)+"°  [G] Grito  [P] Puntería"
                      :"[ESPACIO] Lanzar  [WS] Fuerza  [G] Grito  [P] Puntería");
}

void MainWindow::mensaje(const QString&m,int ms){
    lMsg->setText(m); lMsg->show();
    QTimer::singleShot(ms,this,[=](){lMsg->hide();});
}

void MainWindow::refrescarPunteria() {
    if (!indicador) return;
    auto *pedro = getPedro();
    Roca *roca  = nivelActual ? nivelActual->getRoca() : nullptr;
    if (!pedro || !roca || !pusteriaVisible) return;

    if (modoParabolico)
        indicador->mostrarParabolico(roca->x(), roca->y(),
                                     pedro->fuerzaCargada, angulo);
    else
        indicador->mostrarRecto(roca->x(), roca->y(), pedro->fuerzaCargada);
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if(enMenu){
        if(e->key()==Qt::Key_Return||e->key()==Qt::Key_Enter
            ||e->key()==Qt::Key_Space) iniciarJuego();
        return;
    }
    if(!nivelActual) return;
    auto*pedro=getPedro();
    Roca*roca=nivelActual->getRoca();
    if(!roca) return;

    switch(e->key()){

    case Qt::Key_Space:
        if(!roca->estaActiva&&!esperando) procesarTiro();
        break;

    case Qt::Key_W:
        if(pedro){
            if(modoParabolico){
                angulo=qBound(10.0f, angulo+4.0f, 80.0f);
                mensaje("Ángulo: "+QString::number((int)angulo)+"°",400);
            } else {
                pedro->cargarFuerza(0.8f);
                mensaje("Fuerza: "+QString::number((int)pedro->fuerzaCargada),400);
            }
            refrescarPunteria();
            actualizarHUD();
        }
        break;

    case Qt::Key_S:
        if(pedro){
            if(modoParabolico){
                angulo=qBound(10.0f, angulo-4.0f, 80.0f);
                mensaje("Ángulo: "+QString::number((int)angulo)+"°",400);
            } else {
                pedro->cargarFuerza(-0.8f);
                mensaje("Fuerza: "+QString::number((int)pedro->fuerzaCargada),400);
            }
            refrescarPunteria();
            actualizarHUD();
        }
        break;

    case Qt::Key_G:
        if(pedro&&!roca->estaActiva){
            if(!pedro->gritoDisponible){
                mensaje("🦴 ¡Grito ya usado! Solo 1 por nivel",1500); break;
            }
            pedro->gritoGuerra();
            mensaje("🦴  ¡¡YABBA DABBA DOO!!  — ¡Super Roca activada!",2200);
            actualizarHUD();
        }
        break;

    case Qt::Key_P:
        if(pedro&&!roca->estaActiva){
            if(pusteriaVisible){
                // Toggle: ocultar
                indicador->ocultar();
                pusteriaVisible=false;
                break;
            }
            if(!pedro->pusteriaDisponible){
                mensaje("🎯 ¡Puntería ya usada! Solo 1 por nivel",1500); break;
            }
            pedro->pusteriaDisponible=false;
            pusteriaVisible=true;
            refrescarPunteria();
            mensaje("🎯  Puntería activada — ajusta con ↑↓",1800);
            actualizarHUD();
        }
        break;

    default: break;
    }
}

void MainWindow::procesarTiro() {
    if(juego->tirosRestantes<=0) return;

    Roca* roca = nivelActual->getRoca();
    auto* pedro = getPedro();
    float f = pedro ? pedro->fuerzaCargada : 9.0f;

    pedro->lanzarRoca();
    sndLanzamiento->setPosition(0);
    sndLanzamiento->play();
    tiroPendiente = true;

    if(indicador){
        indicador->ocultar();
        pusteriaVisible = false;
    }

    juego->tirosRestantes--;
    esperando = true;
    actualizarHUD();

    QTimer::singleShot(1380, this, [=]() {

        if(modoParabolico){
            roca->lanzarParabolico(pedro, angulo);
        } else {
            roca->lanzar(f);
        }

        tVerif->start(5000);
    });
}

void MainWindow::onPinoGolpeado(){
    juego->agregarPuntos(100);
    mensaje("💥  +100 puntos",650);
    actualizarHUD();
}
void MainWindow::onRocaDetenida(){
    if(!esperando) return;
    if(!tVerif->isActive()) tVerif->start(1900);
}
void MainWindow::onVerificacion(){
    esperando=false;
    if(nivelActual->nivelCompletado()){ siguiente(); return; }
    if(juego->tirosRestantes<=0){ gameOver(false); return; }

    nivelActual->resetearRoca();
    Roca*r=nivelActual->getRoca();
    if(r){
        connect(r,&Roca::pinoGolpeado,this,&MainWindow::onPinoGolpeado);
        connect(r,&Roca::rocaDetenida,this,&MainWindow::onRocaDetenida);
    }
    pusteriaVisible=false;
    mensaje("Siguiente tiro  →  ESPACIO  para lanzar",1700);
}
void MainWindow::lanzarRocaReal()
{
    if(!tiroPendiente)
        return;

    tiroPendiente = false;

    Roca* roca = nivelActual->getRoca();
    auto* pedro = getPedro();

    if(modoParabolico)
    {
        roca->lanzarParabolico(pedro, angulo);
    }
    else
    {
        float f = pedro->fuerzaCargada;
        roca->lanzar(f);
    }
}

void MainWindow::siguiente(){
    if(auto*p=getPedro()) p->avance(1);
    if(juego->getNivel()==1){
        juego->agregarPuntos(500);
        mensaje("🎉  ¡Nivel 1 completado!  +500 pts bonus",2800);
        juego->siguienteNivel();
        QTimer::singleShot(3000,this,[=](){
            if(indicador){ indicador->ocultar(); }
            delete nivelActual;
            nivelActual=new NivelBarranco(scene);
            nivelActual->cargarNivel();
            modoParabolico=true; angulo=45.0f; pusteriaVisible=false;
            // Resetear habilidades para nivel 2
            if(auto*p=getPedro()){
                p->gritoDisponible=true;
                p->pusteriaDisponible=true;
            }
            conectar(); actualizarHUD();
            mensaje("🦕  Nivel 2 — Lanza en arco con WS para ajustar el ángulo",3000);
        });
    } else {
        gameOver(true);
    }
}

void MainWindow::gameOver(bool v){
    lMsg->hide(); scene->clear();
    if(indicador){ delete indicador; indicador=nullptr; }

    QLinearGradient g(0,0,0,600);
    if(v){g.setColorAt(0,QColor(8,50,8));g.setColorAt(1,QColor(20,85,20));}
    else {g.setColorAt(0,QColor(50,8,8));g.setColorAt(1,QColor(85,18,18));}
    scene->setBackgroundBrush(g);

    scene->addRect(175,105,650,370,
                   QPen(QColor(200,158,48,225),3),QBrush(QColor(0,0,0,192)));
    scene->addLine(215,158,785,158,QPen(QColor(200,158,48,120),1));
    scene->addLine(215,428,785,428,QPen(QColor(200,158,48,120),1));

    auto txt=[&](QString t,QColor c,QFont f,float x,float y){
        auto*i=scene->addText(t); i->setDefaultTextColor(c);
        i->setFont(f); i->setPos(x,y); i->setZValue(3);
    };
    if(v){
        txt("🏆  ¡VICTORIA!",QColor(255,215,48),QFont("Impact",44,QFont::Bold),260,118);
        txt("¡Yabba Dabba Doo!  ¡Pedro lo logró!",Qt::white,QFont("Arial",16),240,228);
    } else {
        txt("💀  GAME OVER",QColor(255,55,55),QFont("Impact",46,QFont::Bold),232,118);
        txt("Se acabaron los tiros...",Qt::white,QFont("Arial",16),295,228);
    }
    txt("Puntuación final:  "+QString::number(juego->getPuntaje())+"  pts",
        QColor(255,212,75),QFont("Arial",20,QFont::Bold),238,295);
    scene->addRect(295,375,410,48,
                   QPen(QColor(200,158,48,205),2),QBrush(QColor(75,45,6,215)));
    auto*vt=scene->addText("Presiona  ENTER  para volver al menú");
    vt->setDefaultTextColor(QColor(220,192,138));
    vt->setFont(QFont("Arial",13)); vt->setPos(310,387); vt->setZValue(4);

    delete nivelActual; nivelActual=nullptr; enMenu=true;
    for(QLabel*l:{lTiros,lPuntos,lNivel,lGrito,lPunteria,lHud}) l->hide();
}

MainWindow::~MainWindow(){ delete ui; }