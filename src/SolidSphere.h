#pragma once

#include "SolidQuad.h"

class CSolidSphere : public CSolid
{
public:
	CSolidSphere(ptr_shader_t pShader, const Vec3f& origin = Vec3f::all(0), float radius = 1, size_t sides = 24)
	{
		// --- PUT YOUR CODE HERE ---
        
        
        
    
        
        Vec3f org(0, 0, 0); // This is the sphere origin
                int height = radius; 
                int height_segments = sides / 2;
        
          
                const Vec3f top(0, radius, 0);
                Vec3f dir(1, 0, 0);
                Vec3f p0 = org + radius * dir;        // This is the Initial point
                Vec3f n0 = dir;
      
        
                Vec3f p1, n1, dir1;
                float t0 = 0;
                
                for (int s = 0; s < sides; s++) {
                    float t1 = static_cast<float>(s + 1) / (sides+1);
                    float beta = -1 * Pif * t1;


                    float h0 = 0;                    // Initial height of the sides
                    for (int h = 0; h < height_segments; h++)
                    {
                        float h1 =  static_cast<float>(h + 1) / sides;
                        float alpha = -2 * Pif * h1;
                        dir1 = Vec3f(cosf(alpha) * cosf(beta), sinf(beta), sinf(alpha) * cosf(beta));

                        p1 = org + radius * dir1;
                        n1 = dir1;
                        
                        
                        Vec3f x = p0 + h0 * top;            //bottom left of the top left
                        Vec3f y = p0 + (h1 * top);
                        Vec3f z = p1 + (h1 * top);
                        Vec3f f = p1 + h0 * top;
                        
                        add(std::make_shared<CSolidQuad>(pShader, x, y, z, f, Vec2f(t0, 1 - h0), Vec2f(t0, 1 - h1), Vec2f(t1, 1 - h1), Vec2f(t1, 1 - h0)));
                        
                        h0 = h1;
                    }

                  
                    if (height >= 1)
                    {
                        add(std::make_shared<CPrimTriangle>(pShader, org, p0, p1, Vec2f(0.5f, 1), Vec2f(t0, 1), Vec2f(t1, 1)));
                        add(std::make_shared<CPrimTriangle>(pShader, org + top, p1 + top, p0 + top, Vec2f(0.5f, 0), Vec2f(t1, 0), Vec2f(t0, 0)));
                    }
                    else if (height <= -1)
                    {
                        add(std::make_shared<CPrimTriangle>(pShader, org, p1, p0, Vec2f(0.5f, 1), Vec2f(t1, 1), Vec2f(t0, 1)));
                        add(std::make_shared<CPrimTriangle>(pShader, org + top, p0 + top, p1 + top, Vec2f(0.5f, 0), Vec2f(t0, 0), Vec2f(t1, 0)));
                    }

                    p0 = p1;
                    n0 = n1;
                    t0 = t1;
                    dir = dir1;
                }
            
        
        
        

	}
	virtual ~CSolidSphere(void) = default;
};
