#pragma once
#include <memory>
#include "render_dx9.h"



class SceneManager : public IRenderable, public ITickable
{
public:
	SceneManager();
	~SceneManager();

	bool init(class RendererDX9& renderer);
	class Space& space();

	virtual void draw(RendererDX9& renderer) override;
	virtual void tick(float deltaTime) override;

private:
	std::unique_ptr<class SkyBox>			m_skybox;
	std::unique_ptr<Space>					m_space;
	std::unique_ptr<class SpaceRenderer>	m_renderer;




};

