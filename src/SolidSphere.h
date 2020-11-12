#pragma once



#include "SolidQuad.h"



class CSolidSphere : public CSolid
{
public:


    CSolidSphere(ptr_shader_t pShader, const Vec3f& origin = Vec3f::all(0), float radius = 1, size_t sides = 24)
    {
        // --- PUT YOUR CODE HERE ---
		Vec3f org(0, 0, 0);
		int height = 1;//height is radius
		int height_segments = sides / 2;

		Vec3f slope(0, radius / height, 0);
		const Vec3f top(0, height, 0);		// The top point
		Vec3f dir(1, 0, 0);					// Initial direction
		Vec3f p0 = org + radius * dir;		// Initial point
		Vec3f n0 = normalize(dir + slope);							// Initial normal
		Vec3f dir1, dir2, p1, n1;						// Next point and normal
		float t0 = 0;						// Initial texture coordinate
		for (int s = 0; s < sides; s++) {
		
			float t1 = static_cast<float>(s + 1) / sides; // Next texture coordinate: [1/sides; 1]
			float beta = -1 * Pif * t1;
			dir1 = Vec3f(cosf(beta), 0, sinf(beta));
			//Vec3f top(0, s/5, 0);

			n1 = normalize(dir1 + slope);

			// Sides
			float h0 = 0;					// Initial height
			for (int h = 0; h < height_segments; h++) {
				float h1 = static_cast<float>(h + 1) / height_segments;	// Next height: [1/height_segments; 1]
				float alpha = -2 * Pif * h1; //range is from 0 to Pi
				dir2 = Vec3f(cosf(alpha) * sinf(beta), cosf(beta), sinf(alpha) * sinf(beta));
				p1 = org + radius * dir2;
				
		
				add(std::make_shared<CSolidQuad>(pShader, p0, p0 + h1 * top, p1 + h1 * top, p1, Vec2f(t0, 1 - h0), Vec2f(t0, 1 - h1), Vec2f(t1, 1 - h1), Vec2f(t1, 1 - h0),n0, n0,n1, n1));
				//add(std::make_shared<CSolidQuad>(pShader, p1, p1 + h1 * top, p0 + h1 * top, p0, Vec2f(t0, 1 - h0), Vec2f(t0, 1 - h1), Vec2f(t1, 1 - h1), Vec2f(t1, 1 - h0)));

				//bottom half
				h0 = h1;
			} // h

			// Caps
			if (height >= 0) {
				add(std::make_shared<CPrimTriangle>(pShader, org, p0, p1, Vec2f(0.5f, 1), Vec2f(t0, 1), Vec2f(t1, 1)));
				add(std::make_shared<CPrimTriangle>(pShader, org + top, p1 + top, p0 + top, Vec2f(0.5f, 0), Vec2f(t1, 0), Vec2f(t0, 0)));
			}
			else {
				add(std::make_shared<CPrimTriangle>(pShader, org, p1, p0, Vec2f(0.5f, 1), Vec2f(t1, 1), Vec2f(t0, 1)));
				add(std::make_shared<CPrimTriangle>(pShader, org + top, p0 + top, p1 + top, Vec2f(0.5f, 0), Vec2f(t0, 0), Vec2f(t1, 0)));
			}
			dir = dir1;
			dir1 = dir2;
			p0 = p1;
			n0 = n1;
			t0 = t1;
		}

    }




    virtual ~CSolidSphere(void) = default;
};