#include "Scene.h"

#include "CameraPerspective.h"

#include "PrimSphere.h"
#include "PrimPlane.h"
#include "PrimTriangle.h"
#include "Solid.h"
#include "SolidQuad.h"
#include "SolidCone.h"
#include "SolidSphere.h"

#include "ShaderFlat.h"
#include "ShaderEyelight.h"
#include "ShaderPhong.h"

#include "Texture.h"

#include "LightOmni.h"
#include "timer.h"


Mat RenderConfusedBarney()
{
    const Size resolution(1200, 600);
    // Define a scene
    CScene scene;
    auto pCamera = std::make_shared<CCameraPerspective>(resolution, Vec3f(0, 10, 55.0f), Vec3f(0, 0, -1), Vec3f(0, 1, 0), 30);
    const std::string barney = "/Users/otmanesabir/Desktop/S5/CG/solutions/eyden-tracer-04/data/barney.obj";
    Mat barneyShirt = imread("/Users/otmanesabir/Desktop/S5/CG/solutions/eyden-tracer-04/data/barney.bmp");
    if (barneyShirt.empty()) printf("ERROR: Texture file is not found!\n");
    auto pTexture = std::make_shared<CTexture>(barneyShirt);
    auto barneyShader = std::make_shared<CShaderEyelight>(pTexture);
    CSolid mrBarney = CSolid(barneyShader, barney);

    scene.add(pCamera);
    scene.add(mrBarney);
    scene.add(std::make_shared<CLightOmni>(Vec3f(100, 100, 100), Vec3f(0, 30, 0), true));
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
	Mat earth = imread("/Users/otmanesabir/Desktop/S5/CG/solutions/eyden-tracer-04/data/1_earth_8k.jpg");
	if (earth.empty()) printf("ERROR: Texture file is not found!\n");
	auto pTexture = std::make_shared<CTexture>(earth);

	// Shaders
	auto tShader = std::make_shared<CShaderEyelight>(pTexture);
    auto pShader = std::make_shared<CShaderEyelight>(RGB(1, 1, 1));

	// Geometry
	CSolidCone solid_cone(tShader, Vec3f(10, -4, 0), 4, 8);
	CSolidSphere solid_sphere(tShader, Vec3f(0, 0, 0), 4, 36);
	auto prim_sphere = std::make_shared<CPrimSphere>(tShader, Vec3f(-10, 0, 0), 4);

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
    imwrite("/Users/otmanesabir/Desktop/S5/CG/solutions/eyden-tracer-04/renders/test_run.jpg", img);
    imshow("Image", img);
    waitKey();
	return 0;
}
