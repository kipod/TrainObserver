#pragma once
#include <memory>
#include "render_interface.h"



class SceneManager : public IRenderable
{
public:
	SceneManager();
	~SceneManager();

	bool init(class RendererDX9& renderer);
	class Space& space();

	virtual void draw(RendererDX9& renderer) override;

	bool initStaticScene(class ConnectionManager& connection);
	bool updateDynamicScene(class ConnectionManager& connection, bool needUpdate, float delta = 1.0f);

private:
	std::unique_ptr<class SkyBox>			m_skybox;
	std::unique_ptr<Space>					m_space;
	std::unique_ptr<class SpaceRenderer>	m_renderer;
};

