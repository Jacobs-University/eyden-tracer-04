#pragma once

#include "Solid.h"

//const int MIN_SECTOR_COUNT = 3;
//const int MIN_STACK_COUNT  = 2;

class CSolidGeosphere : public CSolid
{
public:
    /**
     * @brief Constructor
     * @param pShader Pointer to the shader
     * @param origin The origin of the geosphere
     * @param radius The radius of the geosphere
     * @param sides The number of sides
     */
    CSolidGeosphere(float radius = 1, int sectorCount = 36, int stackCount = 18)
	//CSolidGeosphere(ptr_shader_t pShader, const Vec3f& origin = Vec3f::all(0), float radius = 1, int stacks, int sectors, bool smooth)
	{
		// --- PUT YOUR CODE HERE ---
		// inspired by http://www.songho.ca/opengl/gl_sphere.html
        std::vector<float>().swap(vertices);
        std::vector<float>().swap(normals);
        std::vector<float>().swap(texCoords);

		float x, y, z, xy;
		float nx, ny, nz, lengthInv = 1.0 / radius;
		float s, t;

        float sectorStep = 2 * Pif / sectorCount;
        float stackStep = Pif / stackCount;
        float sectorAngle, stackAngle;

        for(int i = 0; i <= stackCount; ++i) {
            // starting from pi/2 to -pi/2
            stackAngle = Pif / 2 - i * stackStep;
            xy = radius * cosf(stackAngle);
            z = radius * sinf(stackAngle);

            // adding (sectorCount+1) vertices per stack
            // the first and last vertices have same position and normal, but different tex coords
            for(int j = 0; j <= sectorCount; ++j) {
                // starting from 0 to 2pi
                sectorAngle = j * sectorStep;

                x = xy * cosf(sectorAngle);
                y = xy * sinf(sectorAngle);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                // normalized vertex normal (nx, ny, nz)
                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);

                // vertex tex coord (s, t) range between [0, 1]
                s = (float)j / sectorCount;
                t = (float)i / stackCount;
                texCoords.push_back(s);
                texCoords.push_back(t);
            }
        }

        unsigned int k1, k2;
        for(int i = 0; i < stackCount; ++i) {
            k1 = i * (sectorCount + 1);     // beginning of current stack
            k2 = k1 + sectorCount + 1;      // beginning of next stack

            for(int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                // 2 triangles per sector excluding 1st and last stacks
                if(i != 0) {
                    addIndices(k1, k2, k1+1);   // k1---k2---k1+1
                }

                if(i != (stackCount - 1)) {
                    addIndices(k1+1, k2, k2+1); // k1+1---k2---k2+1
                }

                // vertical lines for all stacks
                lineIndices.push_back(k1);
                lineIndices.push_back(k2);
                if(i != 0) {
                    lineIndices.push_back(k1);
                    lineIndices.push_back(k1 + 1);
                }
            }
        }
	}

    CSolidGeosphere(const CSolidGeosphere&) = delete;
    virtual ~CSolidGeosphere(void) = default;
    CSolidGeosphere& operator=(const CSolidGeosphere&) = delete;

protected:
    void  addIndices(unsigned int i1, unsigned int i2, unsigned int i3) {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

private:
    float radius;
    int stackCount = 0;
    int sectorCount = 0;

    std :: vector<float> vertices;
    std :: vector<float> normals;
    std :: vector<float> texCoords;
    std :: vector<unsigned int> indices;
    std :: vector<unsigned int> lineIndices;
};