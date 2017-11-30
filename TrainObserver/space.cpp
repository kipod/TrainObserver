#include <assert.h>
#include "space.h"
#include "json_query_builder.h"
#include "connection_manager.h"
#include "log_interface.h"
#include "space_renderer.h"
#include "math\vector3.h"

using Vector3 = Vector3;

Space::Space():
	m_staticLayerLoaded(false)
{
}


Space::~Space()
{
}

std::shared_ptr<JSONQueryReader> getLayer(const ConnectionManager& connect, SpaceLayer layerId)
{
	JSONQueryWriter writer;
	writer.add("layer", layerId); // STATIC layer

	if (!connect.sendMessage(Action::MAP, &writer.str()))
	{
		LOG(MSG_ERROR, "Failed to create space. Reason: send MAP message failed");
		return nullptr;
	}

	std::string msg;
	if (connect.receiveMessage(msg) != Result::OKEY)
	{
		LOG(MSG_ERROR, "Failed to create space. Reason: receive MAP message failed");
		return nullptr;
	}

	return std::make_shared<JSONQueryReader>(msg);
}

bool Space::initStaticLayer(const ConnectionManager& manager)
{
	if (m_staticLayerLoaded)
		return true;

	auto reader = getLayer(manager, SpaceLayer::STATIC);

	if (reader && reader->isValid())
	{
		m_idx = reader->get<uint>("idx");
		m_name = reader->get<std::string>("name");

		if (!loadLines(*reader))
		{
			LOG(MSG_ERROR, "Failed to create static layer on space. Reason: cannot load lines.");
			return false;
		}

		if (!loadPoints(*reader))
		{
			LOG(MSG_ERROR, "Failed to create static layer on space. Reason: cannot load points.");
			return false;
		}
	}
	else
	{
		LOG(MSG_ERROR, "Failed to create static layer on space. Reason: parcing MAP message failed");
		return false;
	}

	// read geometry coordinates of points
	reader = getLayer(manager, SpaceLayer::COORDINATES);

	if (reader && reader->isValid())
	{
		assert ( m_idx == reader->get<uint>("idx") );

		if (!loadCoordinates(*reader))
		{
			LOG(MSG_ERROR, "Failed to create static layer on space. Reason: cannot load coordinates.");
			return false;
		}
	}
	else
	{
		LOG(MSG_ERROR, "Failed to create static layer on space. Reason: parcing MAP message with coordinates failed");
		return false;
	}

	postCreateStaticLayer();

	m_staticLayerLoaded = true;
	LOG(MSG_NORMAL, "Static space layer created. Points: %d. Lines: %d", m_points.size(), m_lines.size());
	return true;
}

bool Space::updateDynamicLayer(const ConnectionManager& manager)
{
	m_trains.clear();
	m_posts.clear();

	auto reader = getLayer(manager, SpaceLayer::DYNAMIC);

	if (reader && reader->isValid())
	{
		if (!loadTrains(*reader))
		{
			LOG(MSG_ERROR, "Failed to create dynamic layer on space. Reason: cannot load trains.");
			return false;
		}

		if (!loadPosts(*reader))
		{
			LOG(MSG_ERROR, "Failed to create static layer on space. Reason: cannot load posts.");
			return false;
		}
	}
	else
	{
		LOG(MSG_ERROR, "Failed to create dynamic layer on  space. Reason: parcing MAP message failed");
		return false;
	}

	return true;
}

Vector3 coordToVector3(const Coords& c)
{
	return Vector3(float(c.x), 0.0f, float(c.y));
}

void Space::addStaticSceneToRender(SpaceRenderer& renderer)
{
	renderer.setupStaticScene(m_size.x, m_size.y);

	for (const auto& p : m_lines)
	{
		const Line& line = p.second;

		renderer.createRailModel(coordToVector3(line.pt_1->pos), coordToVector3(line.pt_2->pos));
	}

	for (const auto& p : m_points)
	{
		const City& city = p.second;

		renderer.createCity(coordToVector3(city.pos));
	}

}

void Space::addDynamicSceneToRender(SpaceRenderer& renderer)
{
	for (const auto& train : m_trains)
	{
		const Train& t = train.second;
		const Line& line = m_lines.at(t.line_idx);
		float deltaPos = float(t.position) / float(line.length);

		Vector3 p1(float(line.pt_1->pos.x), 0.0f, float(line.pt_1->pos.y));
		Vector3 p2(float(line.pt_2->pos.x), 0.0f, float(line.pt_2->pos.y));

		Vector3 delta(p2 - p1);
		Vector3 position(p1 + delta * deltaPos);
		delta.Normalize();
		if (t.speed < 0)
		{
			delta *= -1.0f;
		}

		auto it = m_trainIdToGraphicsId.find(t.idx);
		if (it == m_trainIdToGraphicsId.end())
		{
			m_trainIdToGraphicsId[t.idx] = renderer.createTrain(position, delta, -1);
		}
		else
		{
			renderer.createTrain(position, delta, it->second);
		}
	}
}

bool Space::loadLines(const JSONQueryReader& reader)
{
	auto values = reader.getValue("line").asArray();
	if (values.size() > 0)
	{
		m_lines.reserve(values.size());
		for (const auto& value : values)
		{
			uint idx = value.get<uint>("idx");
			uint length = value.get<uint>("length");
			auto points = value.getValue("point").asArray();
			uint pid_1, pid_2;

			if (points.size() == 2)
			{
				pid_1 = points[0].get<uint>();
				pid_2 = points[1].get<uint>();
			}
			else
			{
				LOG(MSG_ERROR, "Incorrect format of line!\n");
				return false;
			}

			m_lines.insert(std::make_pair(idx, Line(idx, length, pid_1, pid_2)));
		}
		return true;
	}

	return false;
}

bool Space::loadPoints(const JSONQueryReader& reader)
{
	auto values = reader.getValue("point").asArray();
	if (values.size() > 0)
	{
		m_points.reserve(values.size());
		for (const auto& value : values)
		{
			uint idx = value.get<uint>("idx");
			uint post_id = value.get<uint>("post_id");
			m_points.insert(std::make_pair(idx, City(idx, post_id)));
		}
		return true;
	}

	return false;
}

bool Space::loadTrains(const JSONQueryReader& reader)
{
	auto values = reader.getValue("train").asArray();
	if (values.size() > 0)
	{
		m_trains.reserve(values.size());
		for (const auto& value : values)
		{
			Train train;
			train.idx = value.get<uint>("idx");
			train.line_idx = value.get<uint>("line_idx");
			train.position = value.get<uint>("position");
			train.speed = value.get<uint>("speed");
			train.player_id = value.get<std::string>("player_id");
			m_trains.insert(std::make_pair(train.idx, train));
		}
		return true;
	}

	return false;
}

bool Space::loadPosts(const JSONQueryReader& reader)
{
	auto values = reader.getValue("post").asArray();
	if (values.size() > 0)
	{
		m_trains.reserve(values.size());
		for (const auto& value : values)
		{
			Post post;
			post.idx = value.get<uint>("idx");
			post.armor = value.get<uint>("armor");
			post.population = value.get<uint>("population");
			post.product = value.get<uint>("product");
			post.type = value.get<uint>("type");
			post.name = value.get<std::string>("name");
			m_posts.insert(std::make_pair(post.idx, post));
		}
		return true;
	}

	return false;
}

bool Space::loadCoordinates(const JSONQueryReader& reader)
{
	auto values = reader.getValue("coordinate").asArray();
	if (values.size() > 0)
	{
		assert( values.size() == m_points.size() );
		for (const auto& value : values)
		{
			Coords pos;
			pos.x = value.get<uint>("x");
			pos.y = value.get<uint>("y");
			uint idx = value.get<uint>("idx");
			auto res = m_points.find(idx);
			if (res == m_points.end())
			{
				LOG(MSG_ERROR, "Inconsistent coordinates. Cannot find post with id = %d!", idx);
				return false;
			}

			res->second.pos = pos;
		}

		auto size = reader.getValue("size").asArray();
		assert(size.size() == 2);
		m_size.x = size[0].get<uint>();
		m_size.y = size[1].get<uint>();

		return true;
	}

	return false;
}

void Space::postCreateStaticLayer()
{
	for (auto& p : m_lines)
	{
		Line& line = p.second;
		auto it1 = m_points.find(line.pid_1);
		auto it2 = m_points.find(line.pid_2);

		if (it1 == m_points.end() || it2 == m_points.end())
		{
			LOG(MSG_ERROR, "Incorrect lines vs points connection!");
			return;
		}

		line.pt_1 = &it1->second;
		line.pt_1->lines.emplace_back(&line);
		
		line.pt_2 = &it2->second;
		line.pt_2->lines.emplace_back(&line);
	}
}
