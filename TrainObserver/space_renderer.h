#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "defs.hpp"
#include "model.h"


class SpaceRenderer
{
	struct TrainDesc
	{
		float scale;
		float yOffset;
		float angle;
	};
	struct TrainGeometryData
	{
		class Geometry* geometry;
		TrainDesc desc;

		TrainGeometryData(Geometry* g, const TrainDesc& _data) :
			geometry(g), desc(_data)
		{}

		TrainGeometryData() : geometry(nullptr) {}
	};

	struct TrainModel
	{
		std::unique_ptr<Model> model;
		TrainDesc data;

		TrainModel(Model* m, const TrainDesc& _data) :
			model(m), data(_data)
		{}

		TrainModel() {}
	};

public:
	SpaceRenderer();
	~SpaceRenderer();

	void draw(class RendererDX9& renderer);

	// static scene
	void setupStaticScene(uint x, uint y);
	void createRailModel(const struct Vector3& from, const Vector3& to);
	void createCity(const Vector3& pos);

	// dynamic scene
	void setTrain(const Vector3& pos, const Vector3& dir, int trainId);

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