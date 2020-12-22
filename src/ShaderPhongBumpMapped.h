#pragma once

#include "ShaderFlat.h"
#include "Scene.h"

/**
* @brief Phong Shader with procedural bump mapping
*/

const int nAreaSamples = 1;

class CShaderPhongBumpMapped : public CShaderFlat
{
public:
	/**
	 * @brief Constructor
	 * @param scene Reference to the scene
	 * @param color The color of the object
	 * @param ka The ambient coefficient
	 * @param kd The diffuse reflection coefficients
	 * @param ks The specular refelection coefficients
	 * @param ke The shininess exponent
	 */
	CShaderPhongBumpMapped(CScene& scene, Vec3f color, float ka, float kd, float ks, float ke)
		: CShaderFlat(color)
		, m_scene(scene)
		, m_ka(ka)
		, m_kd(kd)
		, m_ks(ks)
		, m_ke(ke)
	{}
	virtual ~CShaderPhongBumpMapped(void) = default;
  
	virtual Vec3f shade(const Ray& ray) const override
	{
		Vec3f dPdu = (1, 0, 0);
		Vec3f dPdv = (0, 0, 1);
		Vec3f h = ray.org + ray.t * ray.dir;
		float delta_u = 0.5f * cos(3 * h[0] * sin(h[2]));
		float delta_v = 0.5f * sin(13 * h[2]); 

		Vec3f normal0 = ray.hit->getNormal(ray);
		Vec3f normal = normalize(normal0 + delta_u * dPdu + delta_v * dPdv);
		if (normal.dot(ray.dir) > 0) {
			normal = -normal;
		}

		Vec3f reflect = normalize(ray.dir - 2 * normal.dot(ray.dir) * normal);
		Vec3f ambientIntensity(1,1,1);

		Vec3f color = CShaderFlat::shade();
		Vec3f ambientColor = m_ka * color;
		Vec3f res = ambientColor.mul(ambientIntensity);

		Ray shadow;
		shadow.org = ray.org + ray.t * ray.dir;

		for(auto pLight : m_scene.m_vpLights)
			for(int s = 0; s < nAreaSamples; s++) {
				std::optional<Vec3f> lightIntensity = pLight->illuminate(shadow);
				if (lightIntensity) {
					float cosLightNormal = shadow.dir.dot(normal);
					if (cosLightNormal > 0) {
						if (m_scene.occluded(shadow)) {
							continue;
						}

						Vec3f diffuseColor = m_kd * color;
						res += (diffuseColor * cosLightNormal).mul(lightIntensity.value());
					}

					float cosLightReflect = shadow.dir.dot(reflect);
					if (cosLightReflect > 0) {
						Vec3f specularColor = m_ks * RGB(1, 1, 1); // white highlight;
						res += (specularColor * powf(cosLightReflect, m_ke)).mul(lightIntensity.value());
					}
				}
			}

		if (nAreaSamples > 1) {
			res /= nAreaSamples;
		}

		for (int i = 0; i < 3; i++) {
			if (res.val[i] > 1) {
				res.val[i] = 1;
			}
		}
		
		return res;
	}

private:
	CScene& m_scene;
	float 	m_ka;    ///< ambient coefficient
	float 	m_kd;    ///< diffuse reflection coefficients
	float 	m_ks;    ///< specular refelection coefficients
	float 	m_ke;    ///< shininess exponent
};
