#ifndef HELPERS_H
#define HELPERS_H

class Vector3
{
public:
    float x, y, z;
    Vector3(float x, float y, float z) :
        x(x),
        y(y),
        z(z)
    {}
    Vector3() :
		x( 0.0f ),
		y( 0.0f ),
		z( 0.0f )
	{}
};

#endif