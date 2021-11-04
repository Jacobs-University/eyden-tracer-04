// Solid Cone class interface
// Written by Dr. Sergey G. Kosov in 2019 for Project X
#pragma once

#include "SolidQuad.h"

// ================================ Cone Solid Class ================================
/**
 * @brief Cone Solid class
 * @ingroup modulePrimitive
 */
class CSolidCone : public CSolid {
public:
	/**
	 * @brief Constructor
	 * @param pShader Pointer to the shader
	 * @param origin The origin of the cone
	 * @param radius The radius of the cone
	 * @param height The height of the cone
	 * @param sides The number of sides
	 */
	CSolidCone(ptr_shader_t pShader, const Vec3f& origin = Vec3f::all(0), float radius = 1, float height = 1, size_t sides = 24) 
	{
		const Vec3f top(0, height, 0);				// The top point
		Vec3f dir0(1, 0, 0);						// Initial direction
		Vec3f p0 = origin + radius * dir0;			// Initial point
		Vec3f n0 = 0.5 * (normalize(p0 - (origin + top)) + normalize(p0 - origin));
		Vec3f dir1, p1, n1;							// Next point
		float t0 = 0;								// Initial texture coordinate
		for (size_t s = 0; s < sides; s++) {
			float t1 = static_cast<float>(s + 1) / sides; // Next texture coordinate: [1/sides; 1]
			float alpha = -2 * Pif * t1;
			dir1 = Vec3f(cosf(alpha), 0, sinf(alpha));
			p1 = origin + radius * dir1;
			n1 = 0.5 * (normalize(p1 - (origin + top)) + normalize(p1 - origin));
			
			// Sides
			if (height >= 0)
				add(std::make_shared<CPrimTriangle>(pShader, origin + top, p1, p0,
					Vec2f(0, 0), Vec2f(t1, 1), Vec2f(t0, 1),
					Vec3f(0, 1, 0), n1, n0));
			else
				add(std::make_shared<CPrimTriangle>(pShader, origin + top, p0, p1,
					Vec2f(0, 0), Vec2f(t0, 1), Vec2f(t1, 1),
					Vec3f(0, 1, 0), n0, n1));

			// Cap
			if (height >= 0)
				add(std::make_shared<CPrimTriangle>(pShader, origin, p1, p0,
					Vec2f(0, 0), Vec2f(t1, 1), Vec2f(t0, 1),
					Vec3f(0, -1, 0), n1, n0));
			else
				add(std::make_shared<CPrimTriangle>(pShader, origin, p0, p1,
					Vec2f(0, 0), Vec2f(t0, 1), Vec2f(t1, 1),
					Vec3f(0, -1, 0), n0, n1));

			dir0 = dir1;
			p0 = p1;
			n0 = n1;
			t0 = t1;
		}
	}
	virtual ~CSolidCone(void) = default;
};
