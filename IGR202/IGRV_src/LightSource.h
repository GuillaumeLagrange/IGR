#pragma once

#include"Vec3"

class LightSource {
public :
    LightSource();
	LightSource(float x1, float y1, float z1);
//	void setPosition(float x1, float y1, float z1);

private :
    Vec3f position;
};
