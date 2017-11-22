#pragma once
#include <vector>
#include <memory>

namespace graph
{
	struct Vector3;
}

class SpaceRenderer
{
public:
	SpaceRenderer();
	~SpaceRenderer();

	void draw(class RendererDX9& renderer);
	void setupStaticScene();

	void createRailModel(const graph::Vector3& from, const graph::Vector3& to);
	void createCity(const graph::Vector3& pos);

private:
	std::unique_ptr<struct SunLight>	m_sun;
	std::vector<class IRenderable*>		m_staticMeshes;
	std::vector<IRenderable*>			m_dynamicMeshes;
};