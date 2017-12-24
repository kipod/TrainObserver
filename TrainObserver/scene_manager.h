#pragma once
#include <memory>
#include "render_interface.h"
#include "message_interface.h"



class SceneManager : public IRenderable , public IInputListener
{
public:
	SceneManager();
	~SceneManager();

	bool init(class RendererDX9& renderer);
	class Space& space();

	void draw(RendererDX9& renderer) override;

	bool initStaticScene(class ConnectionManager& connection);
	bool updateDynamicScene(class ConnectionManager& connection, int turn);
	void renderDynamicScene(float turnTime);


	virtual void onLMouseUp(int x, int y) override;

private:
	std::unique_ptr<class SkyBox>			m_skybox;
	std::unique_ptr<Space>					m_space;
	std::unique_ptr<class SpaceRenderer>	m_renderer;
};

