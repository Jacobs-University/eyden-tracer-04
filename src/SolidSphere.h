#pragma once

#include "SolidQuad.h"

class CSolidSphere : public CSolid
{
public:
	CSolidSphere(ptr_shader_t pShader, const Vec3f& origin = Vec3f::all(0), float radius = 1, size_t sides = 24)
	{
		const Vec3f top = origin + Vec3f(0, radius, 0);		//the top point
		const Vec3f bottom = origin - Vec3f(0, radius, 0);	//the bottom point
		size_t sliceHeight = static_cast<int>(sides)/2 - 1;
		
		//initial slices points and normals
		Vec3f dir;
		Vec3f p0[sliceHeight];
		Vec3f n0[sliceHeight];
		Vec2f t0[sliceHeight];
		for (int i = 0; i < sliceHeight; i++) {				//rotate over pi, skip end points
			float phi = -1 * Pif * (static_cast<float>(i+1) / (sliceHeight+1));
			dir = Vec3f(sinf(phi), cosf(phi), 0);
			
			p0[i] = origin + radius*dir;
			n0[i] = (p0[i] - origin)/radius;
			t0[i] = Vec2f(0, static_cast<float>(i+1)/(sides/2));
		}
		
		//loop for each vertical slice
		Vec3f p1[sliceHeight];									//next slices points
		Vec3f n1[sliceHeight];
		Vec2f t1[sliceHeight];
		for (size_t s = 0; s < sides; s++) {
			
			//next slices points and normals
			float theta = -2 * Pif * static_cast<float>(s + 1) / sides;
			for (int i = 0; i < sliceHeight; i++) {
				float phi = -1 * Pif * (static_cast<float>(i+1) / (sides/2));
				dir = Vec3f(sinf(phi)*cosf(theta), cosf(phi), sinf(phi)*sinf(theta));
				
				p1[i] = origin + radius*dir;
				n1[i] = (p1[i] - origin)/radius;
				t1[i] = Vec2f(	static_cast<float>(s+1)/sides,
								static_cast<float>(i+1)/(sides/2));
			}

			//top cap
			add(std::make_shared<CPrimTriangle>(pShader, top, p1[0], p0[0],
					Vec2f(0, 0), t1[0], t0[0], Vec3f(0, -1, 0), n1[0], n0[0]));
			
			//vertical slice of sides
			for (int i = 0; i < sliceHeight-1; i++) {
				add(std::make_shared<CSolidQuad>(pShader, p0[i], p0[i+1], p1[i+1], p1[i],
						t0[i], t0[i+1], t1[i+1], t1[i], n0[i], n0[i+1], n1[i+1], n1[i]));
			}
			
			//bottom cap
			add(std::make_shared<CPrimTriangle>(pShader, bottom, p1[sliceHeight-1], p0[sliceHeight-1],
					Vec2f(0, 1), t1[sliceHeight-1], t0[sliceHeight-1],
					Vec3f(0, 1, 0), n1[sliceHeight-1], n0[sliceHeight-1]));

			//transfer points and normals
			for (int i = 0; i < sliceHeight; i++) {
				p0[i] = p1[i];
				n0[i] = n1[i];
				t0[i] = t1[i];
			}
		}
	}
	virtual ~CSolidSphere(void) = default;
};