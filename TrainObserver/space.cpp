#include "space.h"
#include "json_query_builder.h"
#include "connection_manager.h"
#include "log.h"



Space::Space():
	m_valid(false)
{
}


Space::~Space()
{
}

bool Space::init(const ConnectionManager& manager)
{
	if (m_valid)
		return true;

	JSONQueryWriter writer;
	writer.add("layer", 0); // STATIC layer

	if (!manager.sendMessage(Action::MAP, &writer.flush()))
	{
		LOG(MSG_ERROR, L"Failed to create space. Reason: send MAP message failed");
		return false;
	}

	std::string msg;
	if (manager.receiveMessage(msg) != Result::OKEY)
	{
		LOG(MSG_ERROR, L"Failed to create space. Reason: receive MAP message failed");
		return false;
	}


	JSONQueryReader reader(msg);
	if (reader.isValid())
	{
		m_idx = reader.get<unsigned int>("idx");
		m_name = reader.get<std::string>("name");

		if (!loadLines(reader))
		{
			LOG(MSG_ERROR, L"Failed to create space. Reason: cannot load lines.");
			return false;
		}

		if (!loadPoints(reader))
		{
			LOG(MSG_ERROR, L"Failed to create space. Reason: cannot load points.");
			return false;
		}
	}
	else
	{
		LOG(MSG_ERROR, L"Failed to create space. Reason: parcing MAP message failed");
		return false;
	}

	m_valid = true;
	LOG(MSG_NORMAL, L"Static space created. Points: %d. Lines: %d", m_points.size(), m_lines.size());
	return true;
}

bool Space::loadLines(const JSONQueryReader& reader)
{
	auto values = reader.getArray("line");
	if (values.size() > 0)
	{
		m_lines.reserve(values.size());
		for (const auto& value : values)
		{
			unsigned int idx = value.get<unsigned int>("idx");
			unsigned int length = value.get<unsigned int>("length");
			auto points = value.getArray("point");
			unsigned int pid_1, pid_2;

			if (points.size() == 2)
			{
				pid_1 = points[0].get<unsigned int>();
				pid_2 = points[1].get<unsigned int>();
			}
			else
			{
				LOG(MSG_ERROR, L"Incorrect format of line!\n");
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
	auto values = reader.getArray("point");
	if (values.size() > 0)
	{
		m_points.reserve(values.size());
		for (const auto& value : values)
		{
			unsigned int idx = value.get<unsigned int>("idx");
			unsigned int post_id = value.get<unsigned int>("post_id");
			m_points.insert(std::make_pair(idx, Point(idx, post_id)));
		}
		return true;
	}

	return false;
}
