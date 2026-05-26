#include "pedropicapiedra.h"

#include <QPixmap>

PedroPicapiedra::PedroPicapiedra()
{
    QPixmap imagen(":/imagenes/fred.png");

    setPixmap(imagen.scaled(120,120));
}
