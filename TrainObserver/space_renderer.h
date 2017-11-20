#pragma once
#include <vector>
#include <memory>
class SpaceRenderer
{
public:
	SpaceRenderer();
	~SpaceRenderer();

	void draw(class RendererDX9& renderer);
	void setupStaticScene();


private:
	std::unique_ptr<struct SunLight>	m_sun;
	std::vector<class IRenderable*>		m_staticMeshes;
	std::vector<IRenderable*>			m_dynamicMeshes;
};

