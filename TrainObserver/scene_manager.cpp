#include "scene_manager.h"
#include "skybox.h"
#include "space.h"


SceneManager::SceneManager()
{
}


SceneManager::~SceneManager()
{
}

bool SceneManager::init(RendererDX9& renderer)
{
	const wchar_t* skyTextures[] = { 
		L"maps/SkyBox_Front.jpg",
		L"maps/SkyBox_Back.jpg",
		L"maps/SkyBox_Left.jpg",
		L"maps/SkyBox_Right.jpg",
		L"maps/SkyBox_Top.jpg",
		L"maps/SkyBox_Bottom.jpg" };

	bool result = true;

	m_skybox.reset(new SkyBox);
	result |= m_skybox->create(renderer, skyTextures);

	m_space.reset(new Space());

	renderer.addRenderItem(this);

	return result;
}

Space& SceneManager::space()
{
	return *m_space;
}

void SceneManager::draw(RendererDX9& renderer)
{
	m_skybox->draw(renderer);
	m_space->draw(renderer);
}

void SceneManager::tick(float deltaTime)
{
}
