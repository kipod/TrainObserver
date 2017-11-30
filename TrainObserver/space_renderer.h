#pragma once
#include <vector>
#include <memory>
#include "defs.hpp"
#include <unordered_map>

class SpaceRenderer
{
public:
	SpaceRenderer();
	~SpaceRenderer();

	void draw(class RendererDX9& renderer);
	void setupStaticScene(uint x, uint y);

	void createRailModel(const struct Vector3& from, const Vector3& to);
	void createCity(const Vector3& pos);
	int createTrain(const Vector3& pos, const Vector3& dir, int trainId);

private:
	std::unique_ptr<struct SunLight>	m_sun;
	std::vector<class IRenderable*>		m_staticMeshes;
	std::vector<IRenderable*>			m_dynamicMeshes;
	class Model*						m_terrain;

	struct TrainGeometryData
	{
		class Geometry* geometry;
		float scale;
		float yOffset;

		TrainGeometryData(Geometry* g, float scale_, float yOff) :
			geometry(g), scale(scale_), yOffset(yOff)
		{}

		TrainGeometryData() : geometry(nullptr) {}
	};


	std::unordered_map<int, TrainGeometryData> m_trains;
};