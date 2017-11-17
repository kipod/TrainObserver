#pragma once
#include <vector>
class SpaceRenderer
{
public:
	SpaceRenderer();
	~SpaceRenderer();

	void draw(class RendererDX9& renderer);
	void setupStaticScene();


private:
	std::vector<class IRenderable*> m_staticMeshes;
	std::vector<IRenderable*> m_dynamicMeshes;
};

