#include "Scene.h"

#include "CameraPerspective.h"

#include "PrimSphere.h"
#include "PrimPlane.h"
#include "PrimTriangle.h"
#include "Solid.h"
#include "SolidQuad.h"
#include "SolidCone.h"
#include "SolidGeosphere.h"

#include "ShaderFlat.h"
#include "ShaderEyelight.h"
#include "ShaderPhong.h"

#include "Texture.h"

#include "LightOmni.h"
#include "timer.h"

Mat RenderFrame(void)
{
	// Camera resolution
	const Size resolution(1200, 600);
	
	// Define a scene
	CScene scene;

	// Add camera to scene
	auto pCamera = std::make_shared<CCameraPerspective>(resolution, Vec3f(0, 0, -30.0f), Vec3f(0, 0, 1), Vec3f(0, 1, 0), 30);

#ifdef WIN32
	const std::string dataPath = "../data/";
#else
	const std::string dataPath = "../../data/";
#endif

	// Texture
	Mat earth = imread(dataPath + "1_earth_8k.jpg");
	if (earth.empty()) printf("ERROR: Texture file is not found!\n");
	auto pTexture = std::make_shared<CTexture>(earth);

	// Shaders
	auto pShader = std::make_shared<CShaderEyelight>(RGB(0.5f, 1, 0));

	// Geometry
	CSolidCone solid_cone(pShader, Vec3f(10, -4, 0), 4, 8);
	CSolidGeoSphere solid_sphere(pShader, Vec3f(0, 0, 0), 4, 36);
	auto prim_sphere = std::make_shared<CPrimSphere>(pShader, Vec3f(-10, 0, 0), 4);

	// Add everything to the scene
	scene.add(pCamera);
	scene.add(solid_cone);
	scene.add(solid_sphere);
	scene.add(prim_sphere);

	// Build BSPTree
	scene.buildAccelStructure(20, 3);

	Mat img(resolution, CV_32FC3);							// image array
	Ray ray;												// primary ray

	for (int y = 0; y < img.rows; y++)
		for (int x = 0; x < img.cols; x++) {
			scene.getActiveCamera()->InitRay(ray, x, y);	// initialize ray
			img.at<Vec3f>(y, x) = scene.RayTrace(ray);
		}

	img.convertTo(img, CV_8UC3, 255);
	return img;
}

int main(int argc, char* argv[])
{
	DirectGraphicalModels::Timer::start("Rendering...");
	Mat img = RenderFrame();
	DirectGraphicalModels::Timer::stop();
	imshow("Image", img);
	waitKey();
	imwrite("image.jpg", img);
	return 0;
}
