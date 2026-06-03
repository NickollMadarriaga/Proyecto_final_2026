#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QFont>
#include <QGraphicsTextItem>
#include <QLinearGradient>
#include <QPainter>
#include <cmath>

// ── helper para crear labels del HUD ─────────────────────────
static QLabel* mkHUD(QWidget *p, QString t, QString color, int x, int y, int w, int h){
    QLabel *l=new QLabel(t,p);
    l->setStyleSheet(QString(
                         "color:%1;font:bold 13px Arial;"
                         "background:rgba(0,0,0,175);"
                         "padding:3px 9px;border-radius:7px;"
                         "border:1px solid rgba(255,190,60,110);").arg(color));
    l->setGeometry(x,y,w,h);
    l->setAlignment(Qt::AlignCenter);
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

    juego=new JuegoBolos(this);
    nivelActual=nullptr;
    enMenu=true; esperando=false; modoParabolico=false; angulo=-38.0f;

    // HUD
    lTiros    = mkHUD(this,"🪨 Tiros: 5",  "white",   8,  8,125,30);
    lPuntos   = mkHUD(this,"⭐ Pts: 0",   "#FFD700",143,  8,120,30);
    lNivel    = mkHUD(this,"📍 Nivel 1",  "#FF8C00",273,  8,105,30);
    lGrito    = mkHUD(this,"🦴 Grito ✅", "#90EE90",388,  8,120,30);
    lPunteria = mkHUD(this,"🎯 Puntería ✅","#87CEEB",518, 8,135,30);

    lHud=new QLabel("[ESPACIO] Lanzar  [↑↓] Fuerza  [G] Grito  [P] Puntería",this);
    lHud->setStyleSheet("color:#bbb;font:11px Arial;"
                        "background:rgba(0,0,0,155);padding:3px 8px;border-radius:5px;");
    lHud->setGeometry(8,562,490,24);

    lMsg=new QLabel(this);
    lMsg->setStyleSheet("color:white;font:bold 17px Arial;"
                        "background:rgba(0,0,0,205);"
                        "padding:9px 22px;border-radius:13px;"
                        "border:2px solid rgba(255,195,60,210);");
    lMsg->setGeometry(130,250,740,54);
    lMsg->setAlignment(Qt::AlignCenter);
    lMsg->hide();

    tVerif=new QTimer(this);
    tVerif->setSingleShot(true);
    connect(tVerif,&QTimer::timeout,this,&MainWindow::onVerificacion);

    connect(juego,&JuegoBolos::puntuacionActualizada,this,
            [=](int p){lPuntos->setText("⭐ Pts: "+QString::number(p));});
    connect(juego,&JuegoBolos::nivelCambiado,this,
            [=](int n){lNivel->setText("📍 Nivel "+QString::number(n));});

    setWindowTitle("The Flintstones Bowling");
    mostrarMenu();
}

// ═══════════════════════════════════════════════════════════════
//  MENÚ PRINCIPAL PROFESIONAL
// ═══════════════════════════════════════════════════════════════
void MainWindow::mostrarMenu() {
    enMenu=true; scene->clear();

    // Fondo
    QPixmap fm(":/imagenes/menu.png");
    if(!fm.isNull())
        scene->setBackgroundBrush(fm.scaled(1000,600,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    else {
        QLinearGradient g(0,0,0,600);
        g.setColorAt(0,QColor(28,14,4)); g.setColorAt(1,QColor(75,42,12));
        scene->setBackgroundBrush(g);
    }

    // Panel central
    auto *panel=scene->addRect(215,55,570,500,
                                 QPen(QColor(200,158,48,230),3),QBrush(QColor(8,4,1,218)));
    panel->setZValue(1);
    // Adornos
    scene->addLine(245,100,755,100,QPen(QColor(200,158,48,140),1))->setZValue(2);
    scene->addLine(245,500,755,500,QPen(QColor(200,158,48,140),1))->setZValue(2);
    // Esquinas decorativas
    for(int ox:{245,715}) for(int oy:{100,490}){
            scene->addEllipse(ox-5,oy-5,10,10,
                              QPen(Qt::NoPen),QBrush(QColor(200,158,48,180)))->setZValue(3);
        }

    // Título con sombra
    auto txt=[&](QString t,QColor c,QFont f,float x,float y,int z=3){
        auto*i=scene->addText(t); i->setDefaultTextColor(c); i->setFont(f);
        i->setPos(x,y); i->setZValue(z); return i;
    };
    txt("THE FLINTSTONES",QColor(0,0,0,90),QFont("Impact",34,QFont::Bold),262,112,2);
    txt("THE FLINTSTONES",QColor(255,200,48),QFont("Impact",34,QFont::Bold),259,109,3);
    txt("BOWLING",QColor(0,0,0,90),QFont("Impact",52,QFont::Bold),332,152,2);
    txt("BOWLING",QColor(255,108,8),QFont("Impact",52,QFont::Bold),329,149,3);
    txt("¡Yabba Dabba Doo!",QColor(185,215,255,215),QFont("Arial",15),345,218,3);

    // Separador
    scene->addLine(255,250,745,250,QPen(QColor(200,158,48,120),1))->setZValue(3);

    // Controles
    struct Row{QString k,d;};
    QVector<Row> rows={
                         {"[ ESPACIO ]","Lanzar la roca hacia los tótems"},
                         {"[  ↑  ] [  ↓  ]","Aumentar / reducir la fuerza"},
                         {"[   G   ]","Grito de Guerra — Super Roca  (1 uso)"},
                         {"[   P   ]","Puntería mágica — Ver trayectoria  (1 uso)"},
                         {"[ ENTER ]","Iniciar el juego"},
                         };
    int cy=262;
    for(auto&r:rows){
        txt(r.k,QColor(255,208,55),QFont("Courier New",11,QFont::Bold),238,cy,3);
        txt(r.d,QColor(212,212,192),QFont("Arial",11),430,cy,3);
        cy+=38;
    }

    scene->addLine(255,458,745,458,QPen(QColor(200,158,48,120),1))->setZValue(3);

    // Botón JUGAR
    scene->addRect(320,465,360,52,
                   QPen(QColor(255,195,48,225),2),QBrush(QColor(155,65,5,225)))->setZValue(3);
    txt("▶   PRESIONA  ENTER  PARA  JUGAR",
        QColor(255,232,165),QFont("Arial",13,QFont::Bold),332,478,4);

    txt("v1.0  ·  2 Niveles  ·  Agente IA Activo",
        QColor(115,95,65),QFont("Arial",10),320,528,3);

    // Ocultar HUD
    for(QLabel*l:{lTiros,lPuntos,lNivel,lGrito,lPunteria,lHud}) l->hide();
}

// ═══════════════════════════════════════════════════════════════
//  INICIO
// ═══════════════════════════════════════════════════════════════
void MainWindow::iniciarJuego() {
    enMenu=false; juego->iniciar();
    for(QLabel*l:{lTiros,lPuntos,lNivel,lGrito,lPunteria,lHud}) l->show();
    nivelActual=new NivelCueva(scene);
    nivelActual->cargarNivel();
    modoParabolico=false;
    conectar(); actualizarHUD();
    mensaje("🪨  Nivel 1 — ¡Derriba los 5 tótems con 5 tiros!",2400);
}

void MainWindow::conectar() {
    Roca*r=nivelActual?nivelActual->getRoca():nullptr;
    if(!r) return;
    connect(r,&Roca::pinoGolpeado,this,&MainWindow::onPinoGolpeado);
    connect(r,&Roca::rocaDetenida,this,&MainWindow::onRocaDetenida);
}

PedroPicapiedra* MainWindow::getPedro() {
    if(auto*nc=dynamic_cast<NivelCueva*>(nivelActual))    return nc->pedro;
    if(auto*nb=dynamic_cast<NivelBarranco*>(nivelActual)) return nb->fred;
    return nullptr;
}

// ═══════════════════════════════════════════════════════════════
//  HUD
// ═══════════════════════════════════════════════════════════════
void MainWindow::actualizarHUD() {
    lTiros->setText("🪨 Tiros: "+QString::number(juego->tirosRestantes));
    lPuntos->setText("⭐ Pts: "+QString::number(juego->getPuntaje()));
    lNivel->setText("📍 Nivel "+QString::number(juego->getNivel()));
    if(auto*p=getPedro()){
        lGrito->setText(p->gritoDisponible?"🦴 Grito ✅":"🦴 Grito ❌");
        lGrito->setStyleSheet(lGrito->styleSheet());
        lPunteria->setText(p->pusteriaDisponible?"🎯 Puntería ✅":"🎯 Puntería ❌");
    }
    lHud->setText(modoParabolico
                      ?"[ESPACIO] Lanzar  [↑↓] Ángulo  [G] Grito  [P] Puntería"
                      :"[ESPACIO] Lanzar  [↑↓] Fuerza  [G] Grito  [P] Puntería");
}

void MainWindow::mensaje(const QString&m, int ms){
    lMsg->setText(m); lMsg->show();
    QTimer::singleShot(ms,this,[=](){lMsg->hide();});
}

// ═══════════════════════════════════════════════════════════════
//  TECLADO
// ═══════════════════════════════════════════════════════════════
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

    case Qt::Key_Up:
        if(pedro){
            if(modoParabolico){ angulo=qBound(-72.0f,angulo-4.0f,-8.0f);
                mensaje("Ángulo: "+QString::number((int)-angulo)+"°",450); }
            else{ pedro->cargarFuerza(0.8f);
                mensaje("Fuerza: "+QString::number((int)pedro->fuerzaCargada),450); }
            if(pedro->pusteriaActiva) pedro->actualizarPunteria(modoParabolico);
        } break;

    case Qt::Key_Down:
        if(pedro){
            if(modoParabolico){ angulo=qBound(-72.0f,angulo+4.0f,-8.0f);
                mensaje("Ángulo: "+QString::number((int)-angulo)+"°",450); }
            else{ pedro->cargarFuerza(-0.8f);
                mensaje("Fuerza: "+QString::number((int)pedro->fuerzaCargada),450); }
            if(pedro->pusteriaActiva) pedro->actualizarPunteria(modoParabolico);
        } break;

    case Qt::Key_G:
        if(pedro&&!roca->estaActiva){
            if(!pedro->gritoDisponible){
                mensaje("🦴 ¡Grito ya usado! Solo 1 por nivel",1500); break;
            }
            pedro->gritoGuerra();
            mensaje("🦴  ¡¡YABBA DABBA DOO!!  — ¡Super Roca activada!",2200);
            actualizarHUD();
        } break;

    case Qt::Key_P:
        if(pedro&&!roca->estaActiva){
            if(pedro->pusteriaActiva){ pedro->desactivarPunteria(); break; }
            if(!pedro->pusteriaDisponible){
                mensaje("🎯 ¡Puntería ya usada! Solo 1 por nivel",1500); break;
            }
            pedro->activarPunteria();
            pedro->actualizarPunteria(modoParabolico);
            mensaje("🎯  Puntería activada — ¡Elige bien tu tiro!",1800);
            actualizarHUD();
        } break;

    case Qt::Key_Left:
        if(!roca->estaActiva)
            roca->setX(qMax(50.0, roca->x()-20));
        break;

    case Qt::Key_Right:
        if(!roca->estaActiva)
            roca->setX(qMin(400.0, roca->x()+20));
        break;

    default: break;
    }
}

// ═══════════════════════════════════════════════════════════════
//  LANZAMIENTO
// ═══════════════════════════════════════════════════════════════
void MainWindow::procesarTiro() {
    if(juego->tirosRestantes <= 0) return;

    Roca* roca = nivelActual->getRoca();
    auto* pedro = getPedro();

    float f = pedro ? pedro->fuerzaCargada : 9.0f;

    if(pedro)
        pedro->lanzarRoca();

    juego->tirosRestantes--;
    esperando = true;

    if(pedro && pedro->pusteriaActiva)
        pedro->desactivarPunteria();

    actualizarHUD();

    QTimer::singleShot(1840, this, [=]() {

        if(modoParabolico) {
            float rad = angulo * M_PI / 180.0f;

            roca->lanzarParabolico(
                f * std::cos(rad),
                f * std::sin(rad)
                );
        }
        else {
            roca->lanzar(f);
        }

        tVerif->start(4800);
    });
}

// ═══════════════════════════════════════════════════════════════
//  SEÑALES ROCA
// ═══════════════════════════════════════════════════════════════
void MainWindow::onPinoGolpeado() {
    juego->agregarPuntos(100);
    mensaje("💥  +100 puntos",650);
    actualizarHUD();
}
void MainWindow::onRocaDetenida() {
    if(!esperando) return;
    if(!tVerif->isActive()) tVerif->start(1900);
}
void MainWindow::onVerificacion() {
    esperando=false;
    if(nivelActual->nivelCompletado()){ siguiente(); return; }
    if(juego->tirosRestantes<=0){ gameOver(false); return; }
    // Resetear roca
    nivelActual->resetearRoca();
    Roca*r=nivelActual->getRoca();
    if(r){
        connect(r,&Roca::pinoGolpeado,this,&MainWindow::onPinoGolpeado);
        connect(r,&Roca::rocaDetenida,this,&MainWindow::onRocaDetenida);
    }
    mensaje("Siguiente tiro  →  ESPACIO  para lanzar",1700);
}

// ═══════════════════════════════════════════════════════════════
//  SIGUIENTE / FIN
// ═══════════════════════════════════════════════════════════════
void MainWindow::siguiente() {
    if(auto*p=getPedro()) p->avance(1);
    if(juego->getNivel()==1){
        juego->agregarPuntos(500);
        mensaje("🎉  ¡Nivel 1 completado!  +500 pts bonus  — ¡Excelente!",2800);
        juego->siguienteNivel();
        QTimer::singleShot(3000,this,[=](){
            delete nivelActual;
            nivelActual=new NivelBarranco(scene);
            nivelActual->cargarNivel();
            modoParabolico=true; angulo=-38.0f;
            conectar(); actualizarHUD();
            mensaje("🦕  Nivel 2 — El Dino te espera  •  Ajusta el ÁNGULO con ↑↓",3000);
        });
    } else {
        gameOver(true);
    }
}

void MainWindow::gameOver(bool v) {
    lMsg->hide(); scene->clear();
    QLinearGradient g(0,0,0,600);
    if(v){g.setColorAt(0,QColor(8,50,8));g.setColorAt(1,QColor(20,85,20));}
    else {g.setColorAt(0,QColor(50,8,8));g.setColorAt(1,QColor(85,18,18));}
    scene->setBackgroundBrush(g);

    // Panel
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
        txt("¡Yabba Dabba Doo!  ¡Pedro lo logró!",Qt::white,QFont("Arial",16),240,230);
    } else {
        txt("💀  GAME OVER",QColor(255,55,55),QFont("Impact",46,QFont::Bold),232,118);
        txt("Se acabaron los tiros... ¡inténtalo de nuevo!",Qt::white,QFont("Arial",15),195,230);
    }

    txt("Puntuación final:  "+QString::number(juego->getPuntaje())+"  pts",
        QColor(255,212,75),QFont("Arial",20,QFont::Bold),238,295);

    // Botón volver
    scene->addRect(295,375,410,48,
                   QPen(QColor(200,158,48,205),2),QBrush(QColor(75,45,6,215)));
    auto*vt=scene->addText("Presiona  ENTER  para volver al menú");
    vt->setDefaultTextColor(QColor(220,192,138));
    vt->setFont(QFont("Arial",13)); vt->setPos(310,387); vt->setZValue(4);

    delete nivelActual; nivelActual=nullptr; enMenu=true;
    for(QLabel*l:{lTiros,lPuntos,lNivel,lGrito,lPunteria,lHud}) l->hide();
}

MainWindow::~MainWindow(){ delete ui; }