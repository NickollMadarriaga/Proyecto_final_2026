#include "totems.h"

Totems::Totems() {
    QPixmap imagen(":/imagenes/Totem.png");

    setPixmap(imagen.scaled(60,120));
}
