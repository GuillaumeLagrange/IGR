#include "LightSource.h"

LightSource::LightSource()
{
    position = Vec3f(0,0,0);
}

LightSource::LightSource(float x1, float y1, float z1)
{
    position = Vec3f(x1,y1,z1);
}

//void LightSource::setPosition(float x1, float y1, float z1)
//{
//	position
//}
