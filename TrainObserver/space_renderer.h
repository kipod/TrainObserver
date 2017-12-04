#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "defs.hpp"
#include "model.h"


class SpaceRenderer
{
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

	struct TrainModel
	{
		std::unique_ptr<Model> model;
		float scale;
		float yOffset;

		TrainModel(Model* m, float scale_, float yOff) :
			model(m), scale(scale_), yOffset(yOff)
		{}

		TrainModel() {}
	};

public:
	SpaceRenderer();
	~SpaceRenderer();

	void draw(class RendererDX9& renderer);
	void setupStaticScene(uint x, uint y);

	void createRailModel(const struct Vector3& from, const Vector3& to);
	void createCity(const Vector3& pos);
	void moveTrain(const Vector3& pos, const Vector3& dir, int trainId);

	void clearDynamics();

private:
	const TrainGeometryData& loadTrainGeometry();
	TrainModel& getTrain(int trainId);

private:
	std::unique_ptr<struct SunLight>	m_sun;
	std::vector<class IRenderable*>		m_staticMeshes;
	std::vector<IRenderable*>			m_dynamicMeshes;
	Model*								m_terrain;



	std::unordered_map<int, TrainGeometryData>	m_trainModels;
	std::unordered_map<int, TrainModel >		m_trains;
};