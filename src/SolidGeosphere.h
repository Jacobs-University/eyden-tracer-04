#pragma once

#include "SolidQuad.h"

class CSolidGeoSphere: public CSolid
{

public:

CSolidGeoSphere(ptr_shader_t pShader, const Vec3f& origin = Vec3f::all(0), float radius = 1, size_t sides = 24){


float x, y,z, alpha, beta;

for (double alpha = 0; alpha < Pif ; alpha += Pif/sides){


for (double beta = 0; beta < 2.01 * Pif ; beta += Pif / sides){


x = radius * cosf(beta) * sinf (alpha);
y = radius * sinf(beta) * sinf (alpha);
z = radius * cosf (alpha); 

Vec3f p0 = origin+ Vec3f (0, radius, 0);
Vec3f p1 = origin+ Vec3f(x,y,z);


x = radius * cosf(beta) * sinf (alpha+ Pif/sides);
y = radius * sinf(beta) * sinf (alpha+ Pif/sides);
z = radius * cosf (alpha + Pif/sides); 

Vec3f p2 = origin+ Vec3f(x,y,z);


add(std::make_shared<CPrimTriangle>(pShader, p0, p1, p2));


}


}

}



virtual ~CSolidGeoSphere(void) = default;

};