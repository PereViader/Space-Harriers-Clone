#include "Ovni.h"

#include "ScreenBoundTransform.h"

Ovni::Ovni() :
	Enemy(new ScreenBoundTransform(),true)
{
}


Ovni::~Ovni()
{
}
