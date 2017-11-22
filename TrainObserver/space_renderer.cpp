#include "space_renderer.h"
#include "render_interface.h"
#include "model.h"
#include "box.h"
#include "render_dx9.h"
#include "resource_manager.h"

const std::string RAIL_PATH = "meshes/rail/rail.obj";
const std::string SHADER_PATH = "shaders/simple.fx";
const float RAIL_CONNECTION_OFFSET = 0.004f;

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

class SunEffectProperty : public IEffectProperty
{

public:
	SunEffectProperty(const SunLight& sun) :
		IEffectProperty("g_sunLight"),
		m_sun(sun) {}

	virtual bool applyProperty(LPD3DXEFFECT pEffect) const override
	{
		return SUCCEEDED(pEffect->SetValue(m_name.c_str(), &m_sun, sizeof(SunLight)));
	}

private:
	const SunLight& m_sun;

};

SpaceRenderer::SpaceRenderer():
	m_sun(new SunLight)
{
	m_sun->color = graph::Vector3(1.0f, 0.9f, 0.5f); // light yellow 
	m_sun->dir = graph::Vector3(0.2f, -1.0f, 0.3f);
	m_sun->dir.Normalize();
	m_sun->scale = 10.0f;
	m_sun->power = 10.0f;

	RenderSystemDX9::instance().globalEffectProperties().addProperty(GLOBAL, new SunEffectProperty(*m_sun.get()));
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

void SpaceRenderer::createRailModel(const Vector3& from, const Vector3& to)
{
	auto& rs = RenderSystemDX9::instance();
	Model* newModel = new Model();
	Geometry* railGeometry = rs.geometryManager().get(RAIL_PATH);
	Effect* pEffect = rs.effectManager().get(SHADER_PATH);
	newModel->setup(railGeometry, pEffect);

	Vector3 dir(from - to);
	float length = dir.length();
	dir /= length;
	Vector3 center((from + to)* 0.5f);
	Matrix tr; tr.id();

	float angle = dir.z >= 0.0f ? acosf(dir.x) : -acosf(dir.x);

	tr.RotateY(angle + PI*0.5f); // rotate pi/2 because model is pre-rotated horizontally;
	tr.SetTranslation(center);
	tr.Scale(length + RAIL_CONNECTION_OFFSET);
	newModel->setTransform(tr);

	m_staticMeshes.emplace_back(newModel);
}

void SpaceRenderer::createCity(const Vector3& pos)
{

}

void SpaceRenderer::setupStaticScene()
{
	auto& rs = RenderSystemDX9::instance();
	auto device = rs.renderer().device();

	Effect* pEffect = rs.effectManager().get(SHADER_PATH);
	if (!pEffect)
	{
		LOG(MSG_ERROR, "Cannot load shader %s", SHADER_PATH);
		return;
	}

	// TERRAIN
	Model* newModel = new Model();
	Box* pTerrain = new Box();
	pTerrain->create(device);
	newModel->setup(pTerrain, pEffect);
	newModel->effectProperties().setTexture("diffuseTex", "maps/terrain.dds");
	newModel->effectProperties().setTexture("normalTex", "maps/terrain_normal.jpg");

	Matrix transform; transform.id();
	transform.SetTranslation(graph::Vector3(0.0f, -1.0f, 0.0f));
	newModel->setTransform(transform);
	m_staticMeshes.emplace_back(newModel);

	//RAIL
	createRailModel(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.25f, 0.0f, 0.25f));
	createRailModel(Vector3(0.25f, 0.0f, 0.25f), Vector3(0.5f, 0.0f, 0.5f));


}
