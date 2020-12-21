// Plane Geaometrical Primitive class
// Written by Sergey Kosov in 2005 for Rendering Competition
#pragma once

#include "IPrim.h"

// ================================ Infinite Plane Primitive Class ================================
/**
 * @brief The Plane Geometrical Primitive class
 */
class CPrimPlane : public IPrim
{
public:
	/**
	 * @brief Constructor
	 * @param pShader Pointer to the shader to be applied for the prim
	 * @param origin Point on the plane
	 * @param normal Normal to the plane
	 */
	CPrimPlane(ptr_shader_t pShader, Vec3f origin, Vec3f normal)
		: IPrim(pShader)
		, m_normal(normal)
		, m_origin(origin)
	{
		normalize(m_normal);
	}
	virtual ~CPrimPlane(void) = default;

	virtual bool intersect(Ray& ray) const override
	{
		float dist = (m_origin - ray.org).dot(m_normal) / ray.dir.dot(m_normal);
		if (dist < Epsilon || isinf(dist) || dist > ray.t) return false;

		ray.t = dist;
		ray.hit = shared_from_this();
		return true;
	}

	virtual Vec3f getNormal(const Ray& ray) const override
	{
		return m_normal;
	}

	virtual Vec2f getTextureCoords(const Ray& ray) const override
	{
    // copied from  openRT sources 
    Vec3f mu;
		for (int i = 0; i < 3; i++)
			if (fabs(m_normal.val[i]) >= 1.0f / sqrtf(3)) {
				mu.val[i] = 0;
				int mind, maxd;
				if (fabs(m_normal.val[(i + 1) % 3]) > fabs(m_normal.val[(i + 2) % 3])) {
					maxd = (i + 1) % 3;
					mind = (i + 2) % 3;
				}
				else {
					maxd = (i + 2) % 3;
					mind = (i + 1) % 3;
				}
				mu.val[mind] = 1;
				mu.val[maxd] = fabs(m_normal.val[maxd]) > Epsilon ? -m_normal.val[mind] / m_normal.val[maxd] : 0;
				break;
			}
		mu = normalize(mu);
		Vec3f mv = normalize(m_normal.cross(mu));
		Vec3f hp = ray.org + ray.dir * ray.t;
		Vec3f hit = hp;
		Vec3f h = hit - m_origin;
		Vec2f res = norm(h) > Epsilon ? Vec2f(h.dot(mu), h.dot(mv)) : Vec2f(0, 0);
		return res;
        }

	virtual CBoundingBox getBoundingBox(void) const override
	{
		Vec3f minPoint = Vec3f::all(-Infty);
		Vec3f maxPoint = Vec3f::all(Infty);
		for (int i = 0; i < 3; i++)
			if (m_normal.val[i] == 1) {
				minPoint.val[i] = m_origin.val[i];
				maxPoint.val[i] = m_origin.val[i];
				break;
			}
		return CBoundingBox(minPoint, maxPoint);
	}

private:
	Vec3f m_normal;	///< Point on the plane
	Vec3f m_origin;	///< Normal to the plane
};
