#include "TickableObject.h"
#include "Bomberman.h"


TickableObject::TickableObject()
{
    Bomberman::registerObject(this);
}

TickableObject::~TickableObject()
{
    Bomberman::unregisterObject(this);
}