#pragma once

#include "SolidQuad.h"
#include "PrimTriangle.h"
#include "PrimSphere.h"
#include "PrimSphere.h"
#include "PrimPlane.h"
#include "PrimPlane.h"


class CSolidSphere : public CSolid
{
public:
	CSolidSphere(ptr_shader_t pShader, const Vec3f& origin = Vec3f::all(0), float radius = 1, size_t sides = 24)
	{
		// --- PUT YOUR CODE HERE ---
        //taken from the OpenRT github
        size_t height_segments = (sides / 2);
        float t0 = 0;								            // Initial texture coordinate
        float phi0 = 0;
        for (size_t s = 0; s < sides; s++) {
            float t1 = static_cast<float>(s + 1) / sides;       // Next texture coordinate: [1/sides; 1]
            float phi1 = -2 * Pif * t1;

            float h0 = 0.0f / height_segments;					// Initial height
            float theta0 = Pif * (h0 - 0.5f);

            Vec3f n00(cosf(theta0) * cosf(phi0), sinf(theta0), cosf(theta0) * sinf(phi0));

            Vec3f n10(cosf(theta0) * cosf(phi1), sinf(theta0), cosf(theta0) * sinf(phi1));

            bool smooth = true;
            //changing this depending on the task.

            for (size_t h = 0; h < height_segments; h++) {
                float h1 = static_cast<float>(h + 1) / height_segments;		// Next height: [1/height_segments; 1]
                float theta1 = Pif * (h1 - 0.5f);

                Vec3f n01(cosf(theta1) * cosf(phi0), sinf(theta1), cosf(theta1) * sinf(phi0));

                Vec3f n11(cosf(theta1) * cosf(phi1), sinf(theta1), cosf(theta1) * sinf(phi1));

                if (h == 0) { // ----- Bottom cap: triangles -----
                    if (smooth)
                        add(std::make_shared<CPrimTriangle>(pShader,
                            origin + n00 * radius,
                            origin + n11 * radius,
                            origin + n01 * radius,
                            Vec2f(t0, 1 - h0), Vec2f(t1, 1 - h1), Vec2f(t0, 1 - h1),
                            n00, n11, n01));
                    else
                        add(std::make_shared<CPrimTriangle>(pShader,
                            origin + n00 * radius,
                            origin + n11 * radius,
                            origin + n01 * radius,
                            Vec2f(t0, 1 - h0), Vec2f(t1, 1 - h1), Vec2f(t0, 1 - h1)));
                }
                else if (h == height_segments - 1) { // ----- Top cap: triangles -----
                    if (smooth)
                        add(std::make_shared<CPrimTriangle>(pShader,
                            origin + n00 * radius,
                            origin + n10 * radius,
                            origin + n11 * radius,
                            Vec2f(t0, 1 - h0), Vec2f(t1, 1 - h0), Vec2f(t1, 1 - h1),
                            n00, n10, n11));
                    else
                        add(std::make_shared<CPrimTriangle>(pShader,
                            origin + n00 * radius,
                            origin + n10 * radius,
                            origin + n11 * radius,
                            Vec2f(t0, 1 - h0), Vec2f(t1, 1 - h0), Vec2f(t1, 1 - h1)));
                }
                else { // ----- Sides: quads -----
                    if (smooth)
                        add(std::make_shared<CSolidQuad>(pShader,
                            origin + n00 * radius,
                            origin + n10 * radius,
                            origin + n11 * radius,
                            origin + n01 * radius,
                            Vec2f(t0, 1 - h0), Vec2f(t1, 1 - h0), Vec2f(t1, 1 - h1), Vec2f(t0, 1 - h1),
                            n00, n10, n11, n01));
                    else
                        add(std::make_shared<CSolidQuad>(pShader,
                            origin + n00 * radius,
                            origin + n10 * radius,
                            origin + n11 * radius,
                            origin + n01 * radius,
                            Vec2f(t0, 1 - h0), Vec2f(t1, 1 - h0), Vec2f(t1, 1 - h1), Vec2f(t0, 1 - h1)));
                }
                h0 = h1;
                theta0 = theta1;
                n00 = n01;
                n10 = n11;
            } // h
            t0 = t1;
            phi0 = phi1;
        } // s


	}
	virtual ~CSolidSphere(void) = default;
};
