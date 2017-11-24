#pragma once
#include <vector>
#include <memory>
#include "defs.hpp"

class SpaceRenderer
{
public:
	SpaceRenderer();
	~SpaceRenderer();

	void draw(class RendererDX9& renderer);
	void setupStaticScene(uint x, uint y);

	void createRailModel(const struct Vector3& from, const Vector3& to);
	void createCity(const Vector3& pos);

private:
	std::unique_ptr<struct SunLight>	m_sun;
	std::vector<class IRenderable*>		m_staticMeshes;
	std::vector<IRenderable*>			m_dynamicMeshes;
	class Model*						m_terrain;
};