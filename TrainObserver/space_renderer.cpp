#include "space_renderer.h"
#include "render_interface.h"
#include "model.h"
#include "box.h"
#include "render_dx9.h"
#include "resource_manager.h"
#include <fstream>

const std::string RAIL_PATH = "meshes/rail/rail.obj";
const std::string CITY_PATH = "meshes/cities/";
const uint CITY_COUNT = 5;

const std::string SHADER_LIGHTONLY_PATH = "shaders/lightonly.fx";
const std::string SHADER_NORMALMAP_PATH = "shaders/normalmap.fx";
const float RAIL_CONNECTION_OFFSET = 1.0f - 0.01f;
const float RAIL_SCALE = 30.0f;

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
	m_terrain->draw(renderer);

	auto& camera = RenderSystemDX9::instance().renderer().camera();
	camera.beginZBIASDraw(1.001f);

	for (const auto obj : m_staticMeshes)
	{
		obj->draw(renderer);
	}

	for (const auto obj : m_dynamicMeshes)
	{
		obj->draw(renderer);
	}

	camera.endZBIASDraw();

	m_dynamicMeshes.clear();
}

void SpaceRenderer::createRailModel(const Vector3& from, const Vector3& to)
{
	Vector3 dir(to - from);
	float length = dir.length();
	dir /= length;
	float angle = dir.z >= 0.0f ? acosf(dir.x) : -acosf(dir.x);

	uint numTiles = uint(ceilf(length / RAIL_SCALE));
	float tileLength = length / numTiles;

	auto& rs = RenderSystemDX9::instance();
	Geometry* railGeometry = rs.geometryManager().get(RAIL_PATH);
	Effect* pLightonlyEffect = rs.effectManager().get(SHADER_LIGHTONLY_PATH);

	Matrix tr; tr.id();
	tr.RotateY(angle + PI*0.5f); // rotate pi/2 because model is pre-rotated horizontally;
	tr.Scale(tileLength);
	Vector3 tileDelta(dir * (tileLength * RAIL_CONNECTION_OFFSET));
	Vector3 center(from + tileDelta * 0.5f);
	for (uint i = 1; i <= numTiles; ++i)
	{
		Model* newModel = new Model();
		newModel->setup(railGeometry, pLightonlyEffect);

		tr.SetTranslation(center);
		center += tileDelta;
		newModel->setTransform(tr);
		m_staticMeshes.emplace_back(newModel);
	}
}

void SpaceRenderer::createCity(const Vector3& pos)
{
	//return;
	char buf[3];
	static int cityIdx = 0;
	_itoa_s(cityIdx+1, buf, 10);
	std::string dir = CITY_PATH + buf + "/";
	std::string cityPath = dir + "city.obj";
	cityIdx = (cityIdx + 1) % CITY_COUNT;

	auto& rs = RenderSystemDX9::instance();
	Model* newModel = new Model();
	Geometry* cityGeometry = rs.geometryManager().get(cityPath);
	Effect* pLightonlyEffect = rs.effectManager().get(SHADER_LIGHTONLY_PATH);
	newModel->setup(cityGeometry, pLightonlyEffect);
	
	std::string transformPath = dir + "transform.txt";
	std::ifstream fs(transformPath);
	
	float yOffset = 0.0f;
	float scale = 30.0f;
	if (!fs.fail())
	{
		fs >> yOffset;
		fs >> scale;
	}

	Matrix tr; tr.id();
	tr.Scale(scale);
	tr.SetTranslation(Vector3(pos.x, yOffset + pos.y, pos.z));
	newModel->setTransform(tr);
	m_staticMeshes.emplace_back(newModel);
}

void SpaceRenderer::setupStaticScene(uint x, uint y)
{
	auto& rs = RenderSystemDX9::instance();
	auto device = rs.renderer().device();

	Effect* pEffect = rs.effectManager().get(SHADER_NORMALMAP_PATH);
	if (!pEffect)
	{
		LOG(MSG_ERROR, "Cannot load shader %s", SHADER_NORMALMAP_PATH);
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
	transform.SetTranslation(graph::Vector3(float(x)*0.5f, -0.5f, float(y)*0.5f));
	transform.Scale(float(x + 20), 1.0f, float(y + 20));
	newModel->setTransform(transform);
	m_terrain = newModel;
}
