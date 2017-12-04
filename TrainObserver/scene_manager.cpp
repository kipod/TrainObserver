#include "scene_manager.h"
#include "skybox.h"
#include "space.h"
#include "space_renderer.h"


SceneManager::SceneManager():
	m_renderer(new SpaceRenderer())
{
}


SceneManager::~SceneManager()
{
}

bool SceneManager::init(RendererDX9& renderer)
{
	const char* skyTextures[] = { 
		"content/maps/SkyBox_Front.jpg",
		"content/maps/SkyBox_Back.jpg",
		"content/maps/SkyBox_Left.jpg",
		"content/maps/SkyBox_Right.jpg",
		"content/maps/SkyBox_Top.jpg",
		"content/maps/SkyBox_Bottom.jpg" };

	bool result = true;

	m_skybox.reset(new SkyBox);
	result |= m_skybox->create(renderer, skyTextures);

	m_space.reset(new Space());

	return result;
}

Space& SceneManager::space()
{
	return *m_space;
}

void SceneManager::draw(RendererDX9& renderer)
{
	m_skybox->draw(renderer);
	m_renderer->draw(renderer);
}


bool SceneManager::initStaticScene(ConnectionManager& connection)
{
	if (m_space->initStaticLayer(connection))
	{
		m_space->addStaticSceneToRender(*m_renderer);
		return true;
	}

	return false;
}

bool SceneManager::updateDynamicScene(class ConnectionManager& connection, bool needUpdate, float delta)
{
	if (!needUpdate || m_space->updateDynamicLayer(connection))
	{
		m_space->addDynamicSceneToRender(*m_renderer, delta);
		return true;
	}

	return false;
}
