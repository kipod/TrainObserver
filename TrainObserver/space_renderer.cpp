#include "space_renderer.h"
#include "render_interface.h"
#include "model.h"
#include "box.h"
#include "render_dx9.h"
#include "effect_manager.h"



SpaceRenderer::SpaceRenderer()
{
}


SpaceRenderer::~SpaceRenderer()
{
}

void SpaceRenderer::draw(class RendererDX9& renderer)
{
	for (const auto obj : m_staticMeshes)
	{
		obj->draw(renderer);
	}

	for (const auto obj : m_dynamicMeshes)
	{
		obj->draw(renderer);
	}

	m_dynamicMeshes.clear();
}

void SpaceRenderer::setupStaticScene()
{
	Box* pGeometry = new Box();
	pGeometry->create(RenderSystemDX9::instance().renderer().device(), graph::Vector3(10, 20, 30));
	Effect* pEffect = RenderSystemDX9::instance().effectManager().get("shaders/simple.fx");

	Model* newModel = new Model();

	newModel->setup(pGeometry, pEffect);

	m_staticMeshes.emplace_back(newModel);
}
