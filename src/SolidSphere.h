#pragma once

#include "SolidQuad.h"

class CSolidSphere : public CSolid
{
public:
	CSolidSphere(ptr_shader_t pShader, const Vec3f& origin = Vec3f::all(0), float radius = 1, size_t sides = 24)
	{
		// --- PUT YOUR CODE HERE ---
		//use height segments as described in the problem
		size_t height_segments = sides / 2;
		//for our x, we have 2 pi over the sides
		float x = 2 * Pif / sides;
		float y = Pif / height_segments;
		float z = 0;
		float w = 0;
		//nested for loops looping through the sides and the height
		for (size_t s = 0; s < sides; s++) {
			for (size_t h = 0; h < height_segments; h++) {
				/*special cases on top and bottom of the sphere where
				we have just triangles and no quads. With a triangle, we
				need to calculate the three points*/
				if (h == 0) {
					/*taken from cylinder example but modified to fit sphere
					p0 is again the initial point, p1 and p2 the next points.
					*/
					Vec3f p0 = origin + Vec3f(0, radius, 0);
					Vec3f p1 = origin + Vec3f(radius * sinf(y) * cosf(z), radius * cosf(y), radius * sinf(y) * sinf(z));
					Vec3f p2 = origin + Vec3f(radius * sinf(y) * cosf(z + x), radius * cosf(y), radius * sinf(y) * sinf(z + x));
					Vec3f n1 = normalize(p1 - origin);
					Vec3f n2 = normalize(p2 - origin);
					//Vec2f uv1 = getTextureCoords(p1 - origin, radius);
					//Vec2f uv2 = getTextureCoords(p2 - origin, radius);
					//Vec2f uv0 = Vec2f((uv1.val[0] + uv2.val[0]) / 2, 0);

					//idea taken from cylinder but modified to fit sphere
					//first triangle constructor version from problem 1: 
					add(std::make_shared<CPrimTriangle>(pShader, p0, p1, p2));
					
					//extended triangle constructor from problem 2
					//add(std::make_shared<CPrimTriangle>(pShader, p0, p1, p2, normalize(n1 + n2), n1, n2));

					//extended triangle constructor from problem 3
					//add(std::make_shared<CPrimTriangle>(pShader,p0,p1,p2,uv0,uv1,uv2,normalize(n1+n2),n1,n2));
				}
				else if (h == height_segments-1) {
					Vec3f p0 = origin + Vec3f(0, -radius, 0);
					Vec3f p1 = origin + Vec3f(radius * sinf(Pif - y) * cosf(z), radius * sinf(Pif - y) * sinf(z), radius * cosf(Pif - y));
					Vec3f p2 = origin + Vec3f(radius * sinf(Pif - y) * cosf(z + x), radius * cosf(Pif-y), radius * sinf(Pif-y) * sinf(z + x));
					Vec3f n1 = normalize(p1 - origin);
					Vec3f n2 = normalize(p2 - origin);
					//Vec2f uv1 = getTextureCoords(p1 - origin, radius);
					//Vec2f uv2 = getTextureCoords(p2 - origin, radius);
					//Vec2f uv0 = Vec2f((uv1.val[0] + uv2.val[0]) / 2, 1);
					
					//first triangle constructor version from problem 1:
					add(std::make_shared<CPrimTriangle>(pShader, p0, p1, p2));
					//extended triangle constructor from problem 2
					//add(std::make_shared<CPrimTriangle>(pShader, p0, p1, p2, normalize(n1 + n2), n1, n2));
					//constructor for the texture problem
					//add(std::make_shared<CPrimTriangle>(pShader, p0, p1, p2, uv0, uv1, uv2, normalize(n1 + n2), n1, n2)); 
				}
				else {//It's a quad and we need for points
					Vec3f p0 = origin + Vec3f(radius * sinf(w) * cosf(z), radius * cosf(w), radius * sinf(w) * sinf(z));
					Vec3f p1 = origin + Vec3f(radius * sinf(w + y) * cosf(z), radius * cosf(w + y), radius * sinf(w + y) * sinf(z));
					Vec3f p2 = origin + Vec3f(radius * sinf(w + y) * cosf(z + x), radius * cosf(w + y), radius * sinf(w + y) * sinf(z + x));
					Vec3f p3 = origin + Vec3f(radius * sinf(w) * cosf(z + x), radius * cosf(w), radius * sinf(w) * sinf(z + x));
					
					//Vec2f uv0 = getTextureCoords(p0 - origin, radius);
					//Vec2f uv1 = getTextureCoords(p1 - origin, radius);
					//Vec2f uv2 = getTextureCoords(p2 - origin, radius);
					//Vec2f uv3 = getTextureCoords(p3 - origin, radius);
					//add(std::make_shared<CSolidQuad>(pShader, p0, p1, p2, p3, uv0, uv1, uv2, uv3, normalize(p0 - origin), normalize(P1 - origin), normalize(P2 - origin), normalize(P3 - origin)));
					
					//first quad constructor version from problem 1:
					add(std::make_shared<CSolidQuad>(pShader, p0, p1, p2, p3));
					//extended quad constructor from problem 2
					//add(std::make_shared<CSolidQuad>(pShader, p0, p1, p2, p3, normalize(p0 - origin), normalize(p1 - origin), normalize(p2 - origin), normalize(p3 - origin)));
					//extended quad constructor from problem 3
					//add(std::make_shared<CSolidQuad>(pShader, p0, p1, p2, p3, normalize(p0 - origin), normalize(p1 - origin), normalize(p2 - origin), normalize(p3 - origin)));
				}
				w += y;
			}
			z += x;
		}
	}
	virtual ~CSolidSphere(void) = default;
};
