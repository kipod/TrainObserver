#include "space_renderer.h"
#include "render_interface.h"
#include "model.h"
#include "box.h"
#include "render_dx9.h"
#include "effect_manager.h"

struct SunLight
{
	graph::Vector3 dir;
	graph::Vector3 color;
	float  scale;
	float  power;

	SunLight()
	{
		memset(this, 0, sizeof(SunLight));
	}
};

SpaceRenderer::SpaceRenderer():
	m_sun(new SunLight)
{
	m_sun->color = graph::Vector3(1.0f, 0.9f, 0.5f); // light yellow 
	m_sun->dir = graph::Vector3(2.0f, -1.0f, 0.5f);
	m_sun->dir.Normalize();
	m_sun->scale = 10.0f;
	m_sun->power = 10.0f;
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
	auto device = RenderSystemDX9::instance().renderer().device();
	pGeometry->create(device, graph::Vector3(10, 20, 30));
	Effect* pEffect = RenderSystemDX9::instance().effectManager().get("shaders/simple.fx");

	pEffect->setValue(device, "g_sunLight", *m_sun.get());
	pEffect->setTexture(device, "diffuseTex", "maps/terrain.jpg");
	pEffect->setTexture(device, "normalTex", "maps/terrain_normal.jpg");

	Model* newModel = new Model();

	newModel->setup(pGeometry, pEffect);

	Matrix m; m.id();
	//m.Scale(10.0f);
	//newModel->setTransform(m);

	m_staticMeshes.emplace_back(newModel);
}
