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
        /*
        the smoothed cone looks strange because we use triangles for the sides which limits each side
        to only having one normal. this means that in the centre of the side we get clearer shading effect
        than at the top.
        to fix this we need to use more triangles or use quadrons to subdivide the sides at different
        hights to get more normals and clearer shading.

        we could also map onto an intermediate surface like a cylinder or sphere to achieve better continuity.
        */


        // --- PUT YOUR CODE HERE ---
		const Vec3f top(0, height, 0);				// The top point
		Vec3f dir0(1, 0, 0);						// Initial direction
		Vec3f p0 = origin + radius * dir0;			// Initial point

		const Vec3f slope(0, radius / height, 0);	// Slope
		Vec3f n0 = normalize(dir0 + slope);			// Initial normnal
		Vec3f dir1, p1, n1;							// Next point

		float t0 = 0;								// Initial texture coordinate
        int height_segments = sides/2;
		
        for (size_t s = 0; s < sides; s++) {
            float t1 = static_cast<float>(s + 1) / sides; // Next texture coordinate: [1/sides; 1]
            float alpha = -2 * Pif * t1;
            dir1 = Vec3f(cosf(alpha), 0, sinf(alpha));
            p1 = origin + radius * dir1;
            n1 = normalize(dir1 + slope);// next normal

            // Sides
            float h0 = 0; // The initial height
            for (int h = 0; h < height_segments - 1; h++) 
            {
                float h1 = static_cast<float>(h + 1) / height_segments;// Next height: [1/height_segments; 1]
                add(std::make_shared<CSolidQuad>(pShader,
                    p0 + h0 * (top - radius * dir0),
                    p0 + h1 * (top - radius * dir0),
                    p1 + h1 * (top - radius * dir1),
                    p1 + h0 * (top - radius * dir1),
                    Vec2f(t0, 1 - h0), Vec2f(t0, 1 - h1), Vec2f(t1, 1 - h1), Vec2f(t1, 1 - h0),
                    n0, n0, n1, n1));

                h0 = h1;
            }
            if (height >= 0) {
                add(std::make_shared<CPrimTriangle>(pShader,
                    origin + top,
                    p1 + h0 * (top - radius * dir1),
                    p0 + h0 * (top - radius * dir0),
                    Vec2f(0.5f, 0), Vec2f(t1, 1 - h0), Vec2f(t0, 1 - h0),
                    normalize(n0 + n1), n1, n0));
            }
            else {
                add(std::make_shared<CPrimTriangle>(pShader,
                    origin + top,
                    p0 + h0 * (top - radius * dir0),
                    p1 + h0 * (top - radius * dir1),
                    Vec2f(0.5f, 0), Vec2f(t0, 1 - h0), Vec2f(t1, 1 - h0),
                    normalize(n0 + n1), n0, n1));
            }

            // Cap
            if (height >= 0)    add(std::make_shared<CPrimTriangle>(pShader, origin, p1, p0, Vec2f(0.5f, 1), Vec2f(t1, 1), Vec2f(t0, 1), normalize(n0 + n1), n1, n0));
            else                add(std::make_shared<CPrimTriangle>(pShader, origin, p0, p1, Vec2f(0.5f, 1), Vec2f(t0, 1), Vec2f(t1, 1), normalize(n0 + n1), n0, n1));

            dir0 = dir1;
            p0 = p1;
            n0 = n1;
            t0 = t1;
        }
	}
	virtual ~CSolidCone(void) = default;
};
