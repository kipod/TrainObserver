#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "defs.hpp"

struct Line;
class ConnectionManager;
class JSONQueryReader;

struct Coords
{
	uint x = 0;
	uint y = 0;
};

struct City
{
	uint	idx;
	uint	post_id;
	Coords	pos;

	std::vector<const Line*> lines;

	City(uint idx_, uint post_id_) :
		idx(idx_),
		post_id(post_id_)
	{}

};

struct Line
{
	uint idx;
	uint length;
	uint pid_1;
	uint pid_2;

	City*		pt_1;
	City*		pt_2;

	Line(uint idx_, uint length_, uint pid1, uint pid2):
		idx(idx_),
		length(length_),
		pid_1(pid1),
		pid_2(pid2)
	{}
};

struct Train
{
	uint idx;
	uint line_idx;
	std::string player_id;
	uint position;
	uint speed;
};

struct Post
{
	uint idx;
	uint armor;
	std::string name;
	uint population;
	uint product;
	uint type;
};


class Space
{
public:
	Space();
	~Space();

	bool initStaticLayer(const ConnectionManager& manager);
	void updateDynamicLayer(const ConnectionManager& manager);

	void addStaticSceneToRender(class SpaceRenderer& renderer);

private:
	bool loadLines(const JSONQueryReader& reader);
	bool loadPoints(const JSONQueryReader& reader);
	bool loadTrains(const JSONQueryReader& reader);
	bool loadPosts(const JSONQueryReader& reader);
	bool loadCoordinates(const JSONQueryReader& reader);
	void postCreateStaticLayer();
private:
	uint			m_idx;
	std::string		m_name;
	Coords			m_size;
	bool			m_staticLayerLoaded;

	std::unordered_map<uint, City>	m_points;
	std::unordered_map<uint, Line>	m_lines;
	std::unordered_map<uint, Train> m_trains;
	std::unordered_map<uint, Post>	m_posts;

};

