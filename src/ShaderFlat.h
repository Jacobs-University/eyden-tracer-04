#pragma once

#include "IShader.h"
#include "Texture.h"

/**
 * @brief Flat shader class
 */
class CShaderFlat : public IShader
{
public:
	/**
	 * @brief Constructor
	 * @details This is a texture-free and light-source-free shader
	 * @param color The color of the object
	 */
	CShaderFlat(const Vec3f& color) : m_color(color) {}
	/**
	 * @brief Constructor
	 * @details This is a light-source-free shader
	 * @param pTexture Pointer to the texture
	 */
	CShaderFlat(const ptr_texture_t pTexture) : m_pTexture(pTexture) {}

	virtual Vec3f shade(const Ray& ray) const override
	{
        if (m_pTexture) {
            // --- PUT YOUR CODE HERE ---
            if (m_pTexture) {
                // --- PUT YOUR CODE HERE ---
                Vec2f uv = ray.hit->getTextureCoords(ray);
                //return Vec3f(1, 1, 1);
                return m_pTexture->getTexel(uv);
            }
            else
                return m_color;
        }
        else
            return m_color;
    }

private:
	Vec3f m_color;
	const ptr_texture_t m_pTexture = nullptr;
};
