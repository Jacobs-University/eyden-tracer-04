#pragma once

#include "SolidQuad.h"

class CSolidSphere : public CSolid
{
public:
	CSolidSphere(ptr_shader_t pShader, const Vec3f& origin = Vec3f::all(0), float radius = 1, size_t sides = 24)
	{
		// --- PUT YOUR CODE HERE ---
		int TempSize = sides;
		auto r = radius;
		//int n = 5;
		//static_cast<float>
		std::vector<Vec3f> QuadVectors;
		//for counting the height value
		int HeightCounter = 0;
		for (int d = 0; d <= TempSize; d++) {
			//devide y axis into n parts
			auto TempY = ((2 * r * d) / TempSize)  - radius;
			for (int i = 0; i <= TempSize; i++) {
				//devide z axis into n parts
				auto alpha = (2 * Pi * i) / TempSize;
				auto Rprime = sqrt(r * r - TempY * TempY);
				//coordinates of x and z
				auto TempX = Rprime * cos(alpha);
				auto TempZ = Rprime * sin(alpha);
				Vec3f TempOri(TempX, TempY, TempZ);
				TempOri = TempOri + origin;
				QuadVectors.push_back(TempOri);
				std::cout << TempOri.val[0] << " " << TempOri.val[1] << " " << TempOri.val[2] << " " << std::endl;
				//CPrimSphere();
				//add(std::make_shared<CPrimSphere>(pShader, TempOri, 0.05f));
			}
			if (HeightCounter == 1) {
				//up cap
				for (int i = 0; i <= TempSize; i++) {
					add(std::make_shared<CPrimTriangle>(pShader, QuadVectors[0], QuadVectors[TempSize + i], QuadVectors[TempSize + i + 1]));
				}
			}
			else if(HeightCounter == TempSize) {
				//buttom cap
				for (int i = -1; i < TempSize; i++) {
					int TempCounter = (HeightCounter) * TempSize;
					auto Location1 = QuadVectors[QuadVectors.size() - 1];
					auto Location2 = QuadVectors[TempCounter + i];
					auto Location3 = QuadVectors[TempCounter + i + 1];
					std::cout << Location1 << " " << Location2 << " " << Location3 << " " << std::endl;
					add(std::make_shared<CPrimTriangle>(pShader, QuadVectors[QuadVectors.size() - 1], QuadVectors[TempCounter + i], QuadVectors[TempCounter + i + 1]));
				}
			}
			else {
				if (HeightCounter != 0) {
					//skip zero
					int TempCounter = (HeightCounter)*(TempSize + 1);
					int	LastHeight = (HeightCounter - 1) * (TempSize+ 1);
					std::vector<Vec3f> LowerLocationVector;
					std::vector<Vec3f> UppererLocationVector;

					for (int i = 0; i <= TempSize; i++) {
						UppererLocationVector.push_back(QuadVectors[TempCounter + i]);
						LowerLocationVector.push_back(QuadVectors[LastHeight + i]);
					}
					for (int i = 0; i < TempSize; i++) {
						if (i != 0) {
							add(std::make_shared<CSolidQuad>(pShader, UppererLocationVector[i], LowerLocationVector[i - 1], LowerLocationVector[i], UppererLocationVector[i + 1]));
						}
						else {
							add(std::make_shared<CSolidQuad>(pShader, UppererLocationVector[0], LowerLocationVector[TempSize], LowerLocationVector[0], UppererLocationVector[1]));
						}
						//add(std::make_shared<CSolidQuad>(pShader, UppererLocationVector[i], LowerLocationVector[i - 1], LowerLocationVector[i], UppererLocationVector[i + 1]));
					}
					/*int TempCounter = (HeightCounter)*TempSize + 1;
					auto Location1 = QuadVectors[TempCounter + 1];
					auto Location2 = QuadVectors[TempCounter + 2];
					auto Location3 = QuadVectors[TempCounter + 3];
					auto Location4 = QuadVectors[TempCounter + 4];
					auto Location5 = QuadVectors[TempCounter + 5];
					
					int	LastHeight = (HeightCounter - 1)*TempSize + 1;
					auto TLocation1 = QuadVectors[LastHeight + 1];
					auto TLocation2 = QuadVectors[LastHeight + 2];
					auto TLocation3 = QuadVectors[LastHeight + 3];
					auto TLocation4 = QuadVectors[LastHeight + 4];
					auto TLocation5 = QuadVectors[LastHeight + 5];

					add(std::make_shared<CSolidQuad>(pShader, Location1, TLocation5, TLocation1, Location2));
					add(std::make_shared<CSolidQuad>(pShader, Location2, TLocation1, TLocation2, Location3));
					add(std::make_shared<CSolidQuad>(pShader, Location3, TLocation2, TLocation3, Location4));
					add(std::make_shared<CSolidQuad>(pShader, Location4, TLocation3, TLocation4, Location5));
					add(std::make_shared<CSolidQuad>(pShader, Location5, TLocation4, TLocation5, Location1));*/
				}
			}

			HeightCounter++;
		}
		std::cout << "total index " << QuadVectors.size() << std::endl;
		/*add(std::make_shared<CPrimTriangle>(pShader, QuadVectors[1], QuadVectors[20], QuadVectors[30]));

		add(std::make_shared<CPrimTriangle>(pShader, QuadVectors[100], QuadVectors[200], QuadVectors[300]));*/

	}

	void ComputeQuadParam(Vec3f a, Vec3f b, Vec3f c, Vec3f ta, Vec3f tb, Vec3f tc		) {

	}
	// Returns normalized normal vector
	Vec3f calcNormal(float phi, float theta) {
		return Vec3f(cosf(theta) * cosf(phi), sinf(theta), cosf(theta) * sinf(phi));
	}
	
	virtual ~CSolidSphere(void) = default;
};